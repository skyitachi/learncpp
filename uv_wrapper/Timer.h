//
// Created by skyitachi on 2019-05-24.
//

#ifndef LEARNCPP_PEER_H
#define LEARNCPP_PEER_H
#include <uv.h>
#include <memory>
#include <iostream>

class Timer {
public:
  Timer(uv_loop_t* loop):
    m_timer(std::make_unique<uv_timer_t>())
  {
    uv_timer_init(loop, m_timer.get());
    m_timer->data = this;
    uv_timer_start(m_timer.get(), [](uv_timer_t* timer) {
      auto self = (Timer *) timer->data;
      self->periodic();
    }, 1000, 1000);
  }

  void periodic() {
    std::cout << "in the peer staff\n";
  }
  
  ~Timer() {
    uv_timer_stop(m_timer.get());
    auto ptr = m_timer.release();
    uv_close((uv_handle_t* )ptr, [](uv_handle_t* handle) {
      free(handle);
    });
  }
  
private:
  std::unique_ptr<uv_timer_t> m_timer;
};
#endif //LEARNCPP_PEER_H
