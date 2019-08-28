//
// Created by skyitachi on 2019-08-28.
//

#include <zookeeper/zookeeper.h>
#include <pthread.h>
#include <iostream>
#include <string>

volatile bool connected = false;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void zk_event_callback(
    zhandle_t* zh,
    int type,
    int state,
    const char* path,
    void* watcherCtx) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::cout << "zh:" << zh << std::endl;
  std::cout << "type:" << type << std::endl;
  std::cout << "state:" << state << std::endl;
  std::cout << "path:" << path << std::endl;
  std::string *context = static_cast<std::string *>(watcherCtx);
  std::cout << "watcherCtx:" << *context << std::endl;
  delete context;
  
  pthread_mutex_lock(&lock);
  connected = true;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&lock);
}

zhandle_t* initHandle(const char* url , size_t timeout) {
  std::string *str = new std::string("helloworld");
  return zookeeper_init(url, zk_event_callback, timeout, nullptr, str, 0);
}

int main() {
  const char* serverAddr = "localhost:2181/hello";
  const size_t sessionTimeout = 10000; // ms
  const char* rootPath = "/";
  // sequence empheral create
  initHandle(serverAddr, sessionTimeout);
  pthread_mutex_lock(&lock);
  while (!connected) {
    pthread_cond_wait(&cond, &lock);
  }
  pthread_mutex_unlock(&lock);
  return 0;
  
}
