//
// Created by skyitachi on 2018/12/9.
//

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdlib>
#include <stdio.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
struct CustomData {
  int value;
  char name[32];
};

static int posix_fallocate(int fd, off_t offset, off_t len)
{
  off_t c_test;
  int ret;
  if (!__builtin_saddll_overflow(offset, len, &c_test)) {
    fstore_t store = {F_ALLOCATECONTIG, F_PEOFPOSMODE, 0, offset + len};
    // Try to get a continous chunk of disk space
    fcntl(fd, F_PREALLOCATE, &store);
    if (ret < 0) {
      // OK, perhaps we are too fragmented, allocate non-continuous
      store.fst_flags = F_ALLOCATEALL;
      ret = fcntl(fd, F_PREALLOCATE, &store);
      if (ret < 0) {
        return ret;
      }
    }
    ret = ftruncate(fd, offset + len);
  } else {
    // offset+len would overflow.
    ret = -1;
  }
  return ret;
}

int main(int argc, char **argv) {
  int fd = open(argv[1], O_RDWR, 0644);
  int size = sizeof(CustomData);
  if (fd == -1 && errno == ENOENT) {
    // 已经 fallocate 过的不能继续调用 fallocate
    fstore_t fstore1;
    fstore1.fst_flags = F_ALLOCATECONTIG;
    fstore1.fst_posmode = F_PEOFPOSMODE;
    fstore1.fst_length = size;
    fstore1.fst_offset = 0;
    if ((fcntl(fd, F_PREALLOCATE,  &fstore1) < 0)) {
      fprintf(stderr, "fcntl error happens %s\n", strerror(errno));
      exit(-1);
    }
    ftruncate(fd, size);
    printf("allocated: %lld\n", fstore1.fst_bytesalloc);
  } else if (fd < 0) {
    fprintf(stderr, "error happens %s\n", strerror(errno));
    exit(-1);
  }

  char *buf;
  buf = (char *)mmap(0, sizeof(CustomData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (buf == MAP_FAILED) {
    fprintf(stderr, "map failed happens %s\n", strerror(errno));
    exit(-1);
  }
  CustomData *cdPtr = (CustomData *)(buf);
  printf("mmaped object %d, %s\n", cdPtr->value, cdPtr->name);
//  struct CustomData cd;
//  cd.value = 1;
//  strcpy(cd.name, "hello world");
//  memcpy(buf, &cd, sizeof(cd));
  // must call munmap?
  munmap(buf, size);
  close(fd);
  return 0;
}

