#include <iostream>
#include <string>

#include <faiss/index_io.h>
#include <faiss/IndexFlat.h>
#include <faiss/IndexIVFFlat.h>
#include <faiss/impl/io.h>

using idx_t = faiss::Index::idx_t;

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
  std::cout << "after merge_index ------------" << std::endl;
  index1.search(nq, xq, k, D, I);
  for(int i = 0; i < nq; i++) {
    for(int j = 0; j < k; j++) {
      printf("labels = [%ld], distance = [%f]\n", I[i * k + j], D[i * k + j]);
    }
  }

  faiss::write_index_binary(index1, )
}

int main() {
//	faiss::IndexIVFFlat *index = dynamic_cast<faiss::IndexIVFFlat*>(
//		faiss::read_index("data/saved_item.idx"));
//  int d = 8;
//  int nb = 100000;
//  int nq = 10000;
//
//  float *xb = new float[d * nb];
//  for(int i = 0; i < nb; i++) {
//    for(int j = 0; j < d; j++) {
//      xb[d * i + j] = drand48();
//    }
//  }
//
//  faiss::IndexFlatL2 quantizer(d);
//  faiss::IndexIVFFlat index(&quantizer, d, 32);
//  index.train(nb, xb);
//  index.add(nb, xb);
//
//
//  float *xq = new float[d * nq];
//  for(int i = 0; i < nq; i++) {
//    for(int j = 0; j < d; j++) {
//      xq[d * i + j] = drand48();
//    }
//  }
//
//  int k = 4;
//  idx_t *I = new idx_t[k * nq];
//  float *D = new float[k * nq];
//
//  index.search(nq, xq, k, D, I);
//
//  for(int i = 0; i < 3; i++) {
//    printf("Query %d:", i);
//    for(int k = 0; k < d; k++) {
//      printf(" %g", xq[i * d + k]);
//    }
//    printf("\n");
//
//    for(int j = 0; j < k; j++) {
//      int idx = I[i * k + j];
//      printf("  %d (distance=%g) origin vector: ", idx, D[i * k + j]);
//      for (int k = 0; k < d; k++) {
//        printf(" %g", xb[idx * d + k]);
//      }
//      printf("\n");
//    }
//  }
//
//  delete[] xb;
//  delete[] xq;
//  delete[] I;
//  delete[] D;

  merge_index_demo();

	return 0;
}