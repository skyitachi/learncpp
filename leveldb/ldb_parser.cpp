//
// Created by skyitachi on 2019-10-17.
//

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
  const char *file_path = "db/000003.ldb";
  char buf[4096];
  int fd = ::open(file_path, O_RDONLY, 0);
  if (fd < 0) {
    std::cerr << strerror(fd) << std::endl;
    return 1;
  }
  int n;
  while((n = read(fd, buf, 4096)) != -1) {
  
  }
}

