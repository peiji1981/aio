// Copyright (c) 2020 Alipay Inc. All rights reserved.
//
// Brief: A simple elapsed timer.

#pragma once

#include <chrono>

namespace aio {

class ElapsedTimer {
 private:
  std::chrono::system_clock::time_point start_;

 public:
  ElapsedTimer() : start_(std::chrono::system_clock::now()) {}

  void Reset() { start_ = std::chrono::system_clock::now(); }

  double CountSec() {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> ret = now - start_;
    start_ = now;
    return ret.count();
  }
};

}  // namespace aio