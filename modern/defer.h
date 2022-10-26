//
// Created by Shiping Yao on 2022/10/26.
//

#ifndef LEARNCPP_DEFER_H
#define LEARNCPP_DEFER_H

#include <functional>

class DeferHelper
{
public:
  DeferHelper(const std::function<void()> defer) : defer_(std::move(defer))
  {}

  ~DeferHelper()
  {
    if (defer_) {
      defer_();
    }
  }

private:
  const std::function<void()> defer_;
};

#define DERFER_NAME(suffix) defer_helper_##suffix
#define DEFER(callback) DeferHelper DERFER_NAME(__LINE__)(callback)


#endif //LEARNCPP_DEFER_H
