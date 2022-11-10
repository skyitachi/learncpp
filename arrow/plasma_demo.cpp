//
// Created by Shiping Yao on 2022/11/10.
//

#include <iostream>
#include <plasma/client.h>
#include <thread>
#include <chrono>

using namespace plasma;

int main() {
  PlasmaClient client;
  client.Connect("/tmp/plasma");
  ObjectID object_id = ObjectID::from_binary("00000000000000000010");
  int64_t data_size = 1000;
  std::shared_ptr<Buffer> data;
  std::string metadata = "{'author': 'john'}";

//  client.Create(object_id, data_size, (uint8_t*) metadata.data(), metadata.size(), &data);
  client.Create(object_id, data_size, NULL, 0, &data);


  auto d = data->mutable_data();
  for (int64_t i = 0; i < data_size; i++) {
    d[i] = static_cast<uint8_t>(i % 4);
  }

  client.Seal(object_id);

  using namespace std::literals::chrono_literals;
  std::this_thread::sleep_for(10s);
  // Disconnect the client.
  client.Disconnect();
}
