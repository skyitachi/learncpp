//
// Created by skyitachi on 2019-05-24.
//

#include <iostream>
#include "Timer.h"
#include "TcpServer.h"
#include "TcpConnection.h"

int main() {
  Timer timer(uv_default_loop());
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

