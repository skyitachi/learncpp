//
// Created by skyitachi on 2019-05-24.
//

#ifndef LEARNCPP_TCPCONNECTION_H
#define LEARNCPP_TCPCONNECTION_H
#include <uv.h>
#include <memory>

class TcpConnection {
public:
  TcpConnection(uv_loop_t* loop): tcp_(std::make_unique<uv_tcp_t>()) {
    uv_tcp_init(loop, tcp_.get());
    tcp_.get()->data = this;
  }
  
  int Connect(const std::string& host, int port) {
    sockaddr_in sockaddrIn;
    uv_ip4_addr(host.c_str(), port, &sockaddrIn);
    uv_connect_t* connectReq = (uv_connect_t* )malloc(sizeof(uv_connect_t));
    uv_tcp_connect(connectReq, tcp_.get(), (const sockaddr *)&sockaddrIn, [](uv_connect_t* req, int status) {
      free(req);
    });
  }
  
  ~TcpConnection() {
    auto ptr = tcp_.release();
    uv_close((uv_handle_t *)ptr, [](uv_handle_t* handle) {
      free(handle);
    });
  }

private:
  std::unique_ptr<uv_tcp_t> tcp_;
  
};

#endif //LEARNCPP_TCPCONNECTION_H
