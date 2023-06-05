#include <iostream>
#include <string>

#include <faiss/index_io.h>
#include <faiss/IndexFlat.h>
#include <faiss/IndexIVFFlat.h>

using idx_t = faiss::Index::idx_t;


int main() {
//	faiss::IndexIVFFlat *index = dynamic_cast<faiss::IndexIVFFlat*>(
//		faiss::read_index("data/saved_item.idx"));
  int d = 8;
  int nb = 100000;
  int nq = 10000;

  float *xb = new float[d * nb];
  for(int i = 0; i < nb; i++) {
    for(int j = 0; j < d; j++) {
      xb[d * i + j] = drand48();
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

  int k = 4;
  idx_t *I = new idx_t[k * nq];
  float *D = new float[k * nq];

  index.search(nq, xq, k, D, I);

  for(int i = 0; i < 3; i++) {
    printf("Query %d:", i);
    for(int k = 0; k < d; k++) {
      printf(" %g", xq[i * d + k]);
    }
    printf("\n");

    for(int j = 0; j < k; j++) {
      int idx = I[i * k + j];
      printf("  %d (distance=%g) origin vector: ", idx, D[i * k + j]);
      for (int k = 0; k < d; k++) {
        printf(" %g", xb[idx * d + k]);
      }
      printf("\n");
    }
  }

  delete[] xb;
  delete[] xq;
  delete[] I;
  delete[] D;

	return 0;
}