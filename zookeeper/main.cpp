//
// Created by skyitachi on 2019-08-28.
//

#include <zookeeper/zookeeper.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <thread>

volatile bool connected = false;
volatile bool done = false;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t doneCond = PTHREAD_COND_INITIALIZER;

// 任意watch的callback都会call到
// 不是在同一线程callback的
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
  auto* lock = static_cast<pthread_mutex_t* >(watcherCtx);
  
  std::cout << "current thread id " << std::this_thread::get_id() << std::endl;
  if (type == ZOO_SESSION_EVENT) {
    printf("receive zoo_session_event\n");
    // dead lock
    pthread_mutex_lock(lock);
    connected = true;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(lock);
  }
  if (type == ZOO_DELETED_EVENT) {
    printf("path %s deleted\n", path);
  }
  
}

void test(zhandle_t* zh) {
  const char* value = "example";
  char buffer[64] = {0};
  // create 节点
  int ret = zoo_create(zh, "/cppclient", value, strlen(value), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, buffer, sizeof(buffer));
  if (ret != ZOK) {
    printf("zoo_create error %d\n", ret);
  } else {
    printf("create ok\n");
  }
  // sequence and ephemeral
  for (int i = 0; i < 5; i++) {
    ret = zoo_create(zh, "/cppclient_seq", value, strlen(value), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL | ZOO_SEQUENCE, buffer, sizeof(buffer));
    if (ret != ZOK) {
      printf("zoo_create seq and ephemeral error %d\n", ret);
    } else {
      printf("create ok, dir is %s\n", buffer);
    }
  }
  
  // exists
  ret = zoo_exists(zh, "/cppclient_normal", 1, nullptr);
  if (ret == ZNONODE) {
    // normal create
    ret = zoo_create(zh, "/cppclient_normal", value, strlen(value), &ZOO_OPEN_ACL_UNSAFE, 0, buffer, sizeof(buffer));
    if (ret != ZOK) {
      printf("zoo_create seq and ephemeral error %d\n", ret);
    } else {
      printf("create ok, dir is %s\n", buffer);
    }
  } else if (ret == ZOK) {
    printf("znode exists\n");
  }

}

zhandle_t* initHandle(const char* url , size_t timeout) {
//  std::string *str = new std::string("helloworld");
  return zookeeper_init(url, zk_event_callback, timeout, nullptr, &lock, 0);
}
// create node

int main() {
  const char* serverAddr = "localhost:2181/";
  const size_t sessionTimeout = 10000; // ms
  // sequence empheral create
  zhandle_t* zh = initHandle(serverAddr, sessionTimeout);
  pthread_mutex_lock(&lock);
  std::cout << "current thread id " << std::this_thread::get_id() << std::endl;
  while (!connected) {
    // 自动会释放lock
    pthread_cond_wait(&cond, &lock);
  }
  pthread_mutex_unlock(&lock);
  test(zh);
  while (!done) {
    pthread_cond_wait(&doneCond, &lock);
  }
  return 0;
  
}
