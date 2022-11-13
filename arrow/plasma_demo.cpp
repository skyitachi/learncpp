//
// Created by Shiping Yao on 2022/11/10.
//

#include <iostream>
#include <plasma/client.h>
#include <thread>
#include <chrono>
#include <arrow/api.h>
#include <arrow/result.h>
#include <arrow/util/logging.h>
#include <plasma/test_util.h>

using namespace plasma;

void get_object_demo(PlasmaClient& client, ObjectID& oid) {
  ObjectBuffer object_buffer;
  std::cout << "get_demo oid: " << oid.hex() << std::endl;
  ARROW_CHECK_OK(client.Get(&oid, 1, -1, &object_buffer));
  auto buffer = object_buffer.data;
  const uint8_t* data = buffer->data();
  int64_t data_size = buffer->size();
  std::cout << "data_size: " << data_size << std::endl;

  for (int64_t i = 0; i < data_size; i++) {
    ARROW_CHECK(data[i] == static_cast<uint8_t>(i % 4));
  }
}

void write_demo(PlasmaClient& client, ObjectID& oid) {
  // NOTE: 这里不能随意调用
//  ObjectID object_id = random_object_id();
//  std::string id_string = object_id.binary();

  std::cout << "write_demo oid: " << oid.hex() << std::endl;

  bool has_object;
  ARROW_CHECK_OK(client.Contains(oid, &has_object));
  if (has_object) {
    std::cout << "found object " << oid.hex() << std::endl;
    return;
  }
  int64_t data_size = 100;
  std::shared_ptr<Buffer> data;
  ARROW_CHECK_OK(client.Create(oid, data_size, NULL, 0, &data));

  auto d = data->mutable_data();
  for (int64_t i =0; i < data_size; i++) {
    d[i] = static_cast<uint8_t>(i % 4);
  }

  ARROW_CHECK_OK(client.Seal(oid));
}

int main() {
  // NOTE: 和python之间的通信需要使用python client.get_buffers api
  // plasma_store_server 不停的话，数据会一直在
  PlasmaClient client;
  ARROW_CHECK_OK(client.Connect("/tmp/plasma"));
  ObjectID oid = ObjectID::from_binary("00000000000000000000");

  std::thread w_th([&]{
    write_demo(client, oid);
  });
  std::thread r_th([&]{
    get_object_demo(client, oid);
  });
  w_th.join();
  r_th.join();
  ARROW_CHECK_OK(client.Disconnect());
}
