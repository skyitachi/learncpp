//
// Created by Shiping Yao on 2023/2/13.
//

#include <iostream>
#include <zstd.h>
#include <filesystem>
#include <unistd.h>
#include <sys/fcntl.h>

namespace fs = std::filesystem;

std::string create_out_filename(const char *input_filename) {
  return std::string(input_filename) + ".zst";
}

void compress_by_zstd(const std::string& input, const std::string& output) {
  fs::path in = input;
  auto input_sz = fs::file_size(in);
  auto fd = open(input.c_str(), O_RDONLY);
  if (fd < 0) {
    std::cerr << "open input file error " << std::endl;
    return;
  }
  void *const input_buf = malloc(input_sz);
  auto ret = read(fd, input_buf, input_sz);
  if (ret < 0) {
    std::cerr << "read input file err " << std::endl;
    return;
  }

  auto wanted = ZSTD_compressBound(input_sz);
  std::cout << "input file size: " << input_sz << ", bytes, wanted: " << wanted << std::endl;

  void *const out_buf = malloc(wanted);

  size_t const cSize = ZSTD_compress(out_buf, wanted, input_buf, input_sz, 1);

  auto output_fd = open(output.c_str(), O_WRONLY | O_CREAT);
  if (output_fd < 0) {
    std::cerr << "open output file failed" << std::endl;
    return;
  }
  write(output_fd, out_buf, cSize);
}

int main(int argc, char **argv) {
  const char* const name = argv[0];
  if (argc != 2) {
    printf("wrong arguments\n");
    printf("usage: %s File\n", name);
  }

  const char* const in = argv[1];
  auto out = create_out_filename(in);
  compress_by_zstd(in, out);
}