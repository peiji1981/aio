// Copyright 2021 Ant Group Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.



#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "aio/base/exception.h"

namespace aio {

// dataset in memory format
template <class S>
using ColumnVector = std::vector<S>;

using FloatColumnVector = ColumnVector<float>;

using DoubleColumnVector = ColumnVector<double>;

using StringColumnVector = ColumnVector<std::string>;

typedef std::variant<FloatColumnVector, StringColumnVector, DoubleColumnVector>
    ColumnType;

class ColumnVectorBatch {
 public:
  typedef std::pair<size_t, size_t> Dimension;

 public:
  ColumnVectorBatch() : rows_(0) {}
  ~ColumnVectorBatch() = default;

  void Clear() {
    data_.clear();
    rows_ = 0;
  }

  void Reserve(size_t s) { data_.reserve(s); }

  // col access.
  template <typename S>
  const ColumnVector<S>& Col(size_t index) const {
    AIO_ENFORCE(std::holds_alternative<ColumnVector<S>>(data_[index]));
    return std::get<ColumnVector<S>>(data_[index]);
  }

  // col delete & move able col
  template <typename S>
  ColumnVector<S> Pop(size_t index) {
    AIO_ENFORCE(std::holds_alternative<ColumnVector<S>>(data_[index]));
    ColumnVector<S> ret = std::move(std::get<ColumnVector<S>>(data_[index]));
    data_.erase(data_.begin() + index);
    return ret;
  }

  // Scalar access.
  template <typename S>
  const S& At(size_t row, size_t col) const {
    AIO_ENFORCE(std::holds_alternative<ColumnVector<S>>(data_[col]));
    return std::get<ColumnVector<S>>(data_[col])[row];
  }
  // Scalar modify.
  template <typename S>
  S& At(size_t row, size_t col) {
    AIO_ENFORCE(std::holds_alternative<ColumnVector<S>>(data_[col]));
    return std::get<ColumnVector<S>>(data_[col])[row];
  }

  Dimension Shape() const { return {rows_, data_.size()}; }

  template <typename T>
  void AppendCol(T&& col) {
    size_t r = 0;
    if constexpr (std::is_same_v<T, ColumnType>) {
      r = std::visit([](auto&& arg) { return arg.size(); }, col);
    } else {
      r = col.size();
    }
    AIO_ENFORCE(rows_ == 0 || rows_ == r);
    rows_ = r;
    data_.emplace_back(std::forward<T>(col));
  }

 public:
  static ColumnVectorBatch EmptyBatch(size_t rows) {
    ColumnVectorBatch ret(rows);
    return ret;
  }

 private:
  ColumnVectorBatch(size_t rows) : rows_(rows) {}

 private:
  std::vector<ColumnType> data_;
  size_t rows_;
};

// file format
struct Schema {
  enum Type {
    STRING,
    FLOAT,
    DOUBLE,
  };
  std::vector<Type> feature_types;
  std::vector<std::string> feature_names;
};

}  // namespace aio