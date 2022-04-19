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

#include <exception>

#include "absl/debugging/stacktrace.h"
#include "absl/debugging/symbolize.h"
#include "fmt/format.h"

namespace aio {
namespace internal {

const int kMaxStackTraceDep = 16;

template <typename... Args>
inline std::string Format(Args&&... args) {
  return fmt::format(std::forward<Args>(args)...);
}

// Trick to make Format works with empty arguments.
template <>
inline std::string Format() {
  return "";
}

}  // namespace internal

// NOTE: Currently we are using STL exception tree.
//   |- exception
//       |- logic_error
//       |- runtime_error
//           |- io_error

class Exception : public std::exception {
 public:
  Exception() = default;
  explicit Exception(const std::string& msg) : msg_(msg) {}
  explicit Exception(std::string&& msg) : msg_(std::move(msg)) {}
  explicit Exception(const char* msg) : msg_(msg) {}
  explicit Exception(const std::string& msg, void** stacks, int dep)
      : msg_(msg) {
    for (int i = 0; i < dep; ++i) {
      char tmp[1024];
      const char* symbol = "(unknown)";
      if (absl::Symbolize(stacks[i], tmp, sizeof(tmp))) {
        symbol = tmp;
      }
      stack_trace_.append(fmt::format("#{} {}+{}\n", i, symbol, stacks[i]));
    }
  }
  const char* what() const noexcept override { return msg_.c_str(); }

  const std::string& stack_trace() const noexcept { return stack_trace_; }

 private:
  std::string msg_;
  std::string stack_trace_;
};

#define AIO_ERROR_MSG(...) \
  fmt::format("[{}:{}] {}", __FILE__, __LINE__, fmt::format(__VA_ARGS__))

//
// add absl::InitializeSymbolizer to main function to get
// human-readable names stack trace
//
// Example:
// int main(int argc, char *argv[]) {
//   absl::InitializeSymbolizer(argv[0]);
//   ...
// }
//
#define AIO_THROW(...)                                                      \
  do {                                                                      \
    void* stacks[::aio::internal::kMaxStackTraceDep];                       \
    int dep =                                                               \
        absl::GetStackTrace(stacks, ::aio::internal::kMaxStackTraceDep, 0); \
    throw ::aio::Exception(AIO_ERROR_MSG(__VA_ARGS__), stacks, dep);        \
  } while (false)

//------------------------------------------------------
// ENFORCE
// https://github.com/facebookincubator/gloo/blob/master/gloo/common/logging.h

/**
 * Copyright (c) 2017-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

class EnforceNotMet : public Exception {
 public:
  EnforceNotMet(const char* file, int line, const char* condition,
                const std::string& msg)
      : full_msg_(fmt::format("[Enforce fail at {}:{}] {}. {}", file, line,
                              condition, msg)) {}
  EnforceNotMet(const char* file, int line, const char* condition,
                const std::string& msg, void** stacks, int dep)
      : Exception(msg, stacks, dep),
        full_msg_(fmt::format("[Enforce fail at {}:{}] {}. {}", file, line,
                              condition, msg)) {}

  const char* what() const noexcept override { return full_msg_.c_str(); }

 private:
  std::string full_msg_;
};

#define AIO_ENFORCE(condition, ...)                                            \
  do {                                                                         \
    if (!(condition)) {                                                        \
      void* stacks[::aio::internal::kMaxStackTraceDep];                        \
      int dep =                                                                \
          absl::GetStackTrace(stacks, ::aio::internal::kMaxStackTraceDep, 0);  \
      throw ::aio::EnforceNotMet(__FILE__, __LINE__, #condition,               \
                                 ::aio::internal::Format(__VA_ARGS__), stacks, \
                                 dep);                                         \
    }                                                                          \
  } while (false)

}  // namespace aio
