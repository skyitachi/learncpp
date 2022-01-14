//
// Created by skyitachi on 2019-10-17.
//

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <boost/crc.hpp>
#include <leveldb/db.h>

const char* decodeVarint32(const char* buf, const char* limit, uint32_t* value) {
  if (buf < limit) {
    uint32_t result = *(reinterpret_cast<const uint8_t *>(buf));
    if ((result & 128) == 0) {
      std::cout << " small 128 " << std::endl;
      *value = result;
      return buf + 1;
    }
  }
}

void parseInternalKey(const std::string& internalKey) {
  for(int i = 0; i < internalKey.size(); i++) {
    printf("%02x ", internalKey.data()[i]);
  }
  printf("\n");
  const int metaLength = 8;
  auto keySize = internalKey.length() - metaLength;
  const std::string realKey(internalKey.begin(), internalKey.begin() + keySize);
  std::cout << "realKey: " << realKey << std::endl;
  auto begin = internalKey.begin();
  // 小端应该读取第二位
  auto last = begin + keySize;
  auto type = static_cast<uint8_t >(*last) & 0xff;
  if (type == 1) {
    std::cout << "put value type \n";
  } else {
    std::cout << "delete value type \n";
  }
}

int main() {
  const char *file_path = "db/000005.ldb";
  const int kBufSize = 65536;
  char buf[kBufSize];
  int fd = ::open(file_path, O_RDONLY, 0);
  if (fd < 0) {
    std::cerr << "open file error: " << strerror(errno) << std::endl;
    return 1;
  }
  int n;
  int count = 0;
  
  while((n = read(fd, buf, 65536)) != -1) {
    uint32_t shared = 0;
    const char* next = decodeVarint32(buf, buf + kBufSize, &shared);
    std::cout << "shared: " << shared << std::endl;
    uint32_t nonShared = 0;
    next = decodeVarint32(next, buf + kBufSize, &nonShared);
    uint32_t valueSize = 0;
    next = decodeVarint32(next, buf + kBufSize, &valueSize);
    std::cout << "shared: " << shared << " nonshared: " << nonShared << " value size: " << valueSize << std::endl;
    std::string key(next, nonShared);
    parseInternalKey(key);
    std::string value(next + nonShared, valueSize);
    std::cout << "key = " << key << " value = " << value << std::endl;
    break;
    
  }
  if (n == -1) {
    std::cerr << "read file error: " << strerror(fd) << std::endl;
  }
  
  leveldb::DB *db;
  leveldb::Options options;
  options.compression = leveldb::CompressionType::kNoCompression;
  options.create_if_missing = true;
  
  leveldb::Status status = leveldb::DB::Open(options, "db", &db);
  std::cout << "compression: " << options.compression << std::endl;
  if (!status.ok()) {
    std::cerr << "Unable to open/create test database './testdb'" << std::endl;
    std::cerr << status.ToString() << std::endl;
    return -1;
  }
  leveldb::ReadOptions readOptions;
  std::string value;
  std::string key = "0";
  status = db->Get(readOptions, key, &value);
  std::cout << "read by leveldb: " << std::endl;
  if (status.ok()) {
    std::cout << "key = " << key << " value = " << value << std::endl;
  } else {
    std::cout << "key " << key << " not found\n";
  }
}

