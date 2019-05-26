//
// Created by skyitachi on 2019-05-24.
//

#ifndef LEARNCPP_TCPSERVER_H
#define LEARNCPP_TCPSERVER_H
#include <uv.h>
#include <memory>

class TcpServer {
public:
  TcpServer(uv_loop_t* loop): tcp_(std::make_unique<uv_tcp_t>()) {
    uv_tcp_init(loop, tcp_.get());
    tcp_.get()->data = this;
  }
  
  int Listen(const std::string& host, int port) {
    sockaddr_in sockaddrIn;
    uv_ip4_addr(host.c_str(), port, &sockaddrIn);
    uv_tcp_bind(tcp_.get(), (const sockaddr*)&sockaddrIn, 0);
  }
  
  ~TcpServer() {
    auto ptr = tcp_.release();
    uv_close((uv_handle_t *)ptr, [](uv_handle_t* handle) {
      free(handle);
    });
  }
  
private:
  std::unique_ptr<uv_tcp_t> tcp_;
  
};
#endif //LEARNCPP_TCPSERVER_H
