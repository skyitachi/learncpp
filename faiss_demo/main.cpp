#include <iostream>
#include <faiss/IndexFlat.h>

using idx_t = faiss::Index::idx_t;

int main() {

	int d = 64;
	int nb = 100;
	int nq = 10;
	float *xb = new float[d * nb];
	float *xq = new float[d * nq];
	for (int i = 0; i < nb; i++) {
		for (int j = 0; j < d; j++) xb[d * i + j] = drand48();
		xb[d * i] += i / 1000.;
	}

	for (int i = 0; i < nq; i++) {
		for (int j = 0; j < d; j++) xq[d * i + j] = drand48();

		xq[d *i] += i / 1000.;
	}


	faiss::IndexFlatL2 index(d);
	printf("is_trained = %s\n", index.is_trained ? "true" : "false");
	index.add(nb, xb);

	int k = 4;
	{
		idx_t *I = new idx_t[k * 5];
		float* D = new float[k * 5];

		index.search(5, xb, k, D, I);

		printf("I=\n");
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < k; j++)
                printf("%5zd ", I[i * k + j]);
            printf("\n");
        }

        printf("D=\n");
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < k; j++)
                printf("%7g ", D[i * k + j]);
            printf("\n");
        }

        delete[] I;
        delete[] D;
	}
	return 0;

}