#include <iostream>
#include <string>

#include <faiss/index_io.h>
#include <faiss/IndexFlat.h>
#include <faiss/IndexIVFFlat.h>
#include <faiss/impl/IDSelector.h>
#include <faiss/impl/io.h>
#include <unistd.h>
#include <fcntl.h>
#include <cassert>

#include <thread>

using idx_t = faiss::idx_t;

class ReaderWithFileOffset: public faiss::IOReader {
public:
  int offset;
  ReaderWithFileOffset(std::string filename, int64_t off) {
    offset = off;
    name = filename;
    fd_ = ::open(name.c_str(), O_RDONLY, 0666);
    if (fd_ < 0) {
      std::cout << "open file failed: " << strerror(errno) << std::endl;
      exit(1);
    }
    auto v_off = ::lseek(fd_, offset, SEEK_SET);
    assert(v_off == offset);
  }

  size_t operator()(void* ptr, size_t size, size_t nitems) override {
    auto sz = ::pread(fd_, ptr, size * nitems, offset);
    offset += sz;
    return nitems;
  }

  int filedescriptor() override {
    return fd_;
  }

private:
  int fd_;
};

class WriterWithFileOffset: public faiss::IOWriter {
public:
  int offset;
  WriterWithFileOffset(std::string filename, int64_t off) {
    offset = off;
    name = filename;
    fd_ = ::open(name.c_str(), O_RDWR | O_CREAT, 0666);
    if (fd_ < 0) {
      std::cout << "open file failed: " << strerror(errno) << std::endl;
      exit(1);
    }
    auto v_off = ::lseek(fd_, offset, SEEK_SET);
    std::cout << "set file offset: " << v_off << " file descriptor: "<< fd_ << std::endl;
  }

  int filedescriptor() override {
      return fd_;
  }

  int fileno() {
    return fd_;
  }

  size_t operator()(const void* ptr, size_t size, size_t nitems) override {
//    std::cout << "write index data: " << size << ", nitems: " << nitems << std::endl;
    auto written = pwrite(fd_, ptr, size * nitems, offset);
//    std::cout << "write data successfully: " << written << ", errno message: " << strerror(errno) << std::endl;
    offset += written;
    return nitems;
  };

private:
  int fd_;
};


void merge_index_demo() {
  int d = 4;
  int nb = 2;
  float *xb1 = new float[d * nb];
  float *xb2 = new float[d * nb];
  {
    xb1[0] = 0.0;
    xb1[1] = 0.1;
    xb1[2] = 0.2;
    xb1[3] = 0.3;

    xb1[4] = 1.0;
    xb1[5] = 1.1;
    xb1[6] = 1.2;
    xb1[7] = 1.3;

    xb2[0] = 3.0;
    xb2[1] = 3.1;
    xb2[2] = 3.2;
    xb2[3] = 3.3;

    xb2[4] = 4.0;
    xb2[5] = 4.1;
    xb2[6] = 4.2;
    xb2[7] = 4.3;
  }
  faiss::IndexFlatL2 quantizer(d);
  faiss::IndexIVFFlat index1(&quantizer, d, 1);
  faiss::IndexIVFFlat index2(&quantizer, d, 1);

  index1.train(nb, xb1);
  index1.add(nb, xb1);

  index2.train(nb, xb2);
//  index2.add(nb, xb2);
  idx_t ids[] = {4, 5};
  index2.add_with_ids(nb, xb2, ids);

  int k = 1;
  int nq = 1;
  idx_t *I = new idx_t[k * nq];
  float *D = new float[k * nq];
  float* xq = new float[nq * d];
  xq[0] = 3.0; xq[1] = 3.0; xq[2] = 3.0; xq[3] = 3.0;

  index1.search(nq, xq, k, D, I);
  for(int i = 0; i < nq; i++) {
    for(int j = 0; j < k; j++) {
      printf("index1 search result: labels = [%ld], distance = [%f]\n", I[i * k + j], D[i * k + j]);
    }
  }

  index2.search(nq, xq, k, D, I);
  for(int i = 0; i < nq; i++) {
    for(int j = 0; j < k; j++) {
      printf("index2 search result: labels = [%ld], distance = [%f]\n", I[i * k + j], D[i * k + j]);
    }
  }

  index1.merge_from(index2, 0);

  xq[0] = 4.0; xq[1] = 4.0; xq[2] = 4.0; xq[3] = 4.0;
  index1.search(nq, xq, k, D, I);
  for(int i = 0; i < nq; i++) {
    for(int j = 0; j < k; j++) {
      printf("same thread labels = [%ld], distance = [%f]\n", I[i * k + j], D[i * k + j]);
    }
  }

  std::thread th([&]{
    std::cout << "after merge_index ------------" << std::endl;
    index1.search(nq, xq, k, D, I);
    for(int i = 0; i < nq; i++) {
      for(int j = 0; j < k; j++) {
        printf("different thread labels = [%ld], distance = [%f]\n", I[i * k + j], D[i * k + j]);
      }
    }
  });
  th.join();

  faiss::write_index(&index1, "saved.index");

  auto *writer = new WriterWithFileOffset("saved_offset.index", 128);
  faiss::write_index(&index1, writer);


  ReaderWithFileOffset reader("saved_offset.index", 128);
  auto* index_read = faiss::read_index(&reader, 0);
  std::cout << "index_read: dimension = " << index_read->d << ", ntotal = "<< index_read->ntotal << std::endl;
  assert(index1.is_trained == index_read->is_trained);
  assert(index1.d == index_read->d);
  assert(index1.ntotal == index_read->ntotal);
}

class CustomIDSelector: public faiss::IDSelector {
public:
  bool is_member(idx_t id) const override {
    return id > 10;
  }
};

class CustomSearchParams: public faiss::SearchParameters {
public:
  CustomSearchParams(CustomIDSelector* sel) {
    this->sel = sel;
  }
};

void search_with_params() {
  int d = 3;
  int nb = 1000;
  int nq = 1;

  float *xb = new float[d * nb];
  for(int i = 0; i < nb; i++) {
    for(int j = 0; j < d; j++) {
      xb[d * i + j] = i;
    }
  }
  faiss::IndexFlatL2 quantizer(d);
  faiss::IndexIVFFlat index(&quantizer, d, 32);
  index.train(nb, xb);
  index.add(nb, xb);

  float *xq = new float[d * nq];

  for(int i = 0; i < nq; i++) {
    for(int j = 0; j < d; j++) {
      xq[d * i + j] = drand48();
    }
  }

  xq[0] = 0.0; xq[1] = 0.0; xq[2] = 0.0;


  int k = 3;
  idx_t *I = new idx_t[k * nq];
  float *D = new float[k * nq];
  index.search(1, xq, k, D, I);

  for(int i = 0; i < 1; i++) {
    for(int j = 0; j < k; j++) {
      int idx = I[i * k + j];
      printf("search_idx = %d (distance=%g) origin vector: ", idx, D[i * k + j]);
      for (int k = 0; k < d; k++) {
        printf(" %g", xb[idx * d + k]);
      }
      printf("\n");
    }
  }

  std::cout << "sorted ids: " << index.check_ids_sorted() << std::endl;

  faiss::SearchParametersIVF* params = new faiss::SearchParametersIVF();
  params->sel = new CustomIDSelector();
//  params->quantizer_params = new CustomSearchParams(new CustomIDSelector);

  index.search(1, xq, k, D, I, params);

  for(int i = 0; i < 1; i++) {
    for(int j = 0; j < k; j++) {
      int idx = I[i * k + j];
      printf("search_idx=%d (distance=%g) origin vector: ", idx, D[i * k + j]);
      for (int k = 0; k < d; k++) {
        printf(" %g", xb[idx * d + k]);
      }
      printf("\n");
    }
  }

}

int global_array[100] = {-1};

void double_free() {
  int* a = new int;
  delete a;
  delete a;
}

int *g_ptr;
void leak_demo() {
  g_ptr = new int;
  g_ptr = nullptr;
}

void leak_demo_c() {
  void *p;
  p = malloc(7);
  p = 0; // The memory is leaked here.
}

int main(int argc, char **argv) {
  search_with_params();
  // 都没有用
  leak_demo();
  leak_demo_c();

//  merge_index_demo();
//  return global_array[argc + 100];
  return 0;
}