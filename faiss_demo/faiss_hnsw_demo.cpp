//
// Created by skyitachi on 24-7-21.
//

#include <iostream>
#include <vector>
#include <faiss/IndexHNSW.h>
#include <faiss/index_io.h>
#include <faiss/utils/distances.h>

#include <fstream>

int load_data(std::vector<float> &dest, const std::string& filename) {

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file");
    }

    int32_t dimension;

    size_t offset = 4;
    size_t count = 0;

    while (file) {
        // Read the dimension of the vector
        file.read(reinterpret_cast<char*>(&dimension), sizeof(dimension));
        if (file.eof()) break; // Check for end of file

        dest.resize(count + dimension);

        size_t batch_size = dimension * sizeof(float);

        file.read(reinterpret_cast<char*>(dest.data() + count), batch_size);

        count += dimension;

        offset += batch_size + sizeof(dimension);

    }
    return dimension;
}

int main() {

    std::vector<float> data;
    int dimension = load_data(data, "/home/skyitachi/lab/learnpython/data/sift/sift_base.fvecs");

    std::cout << "dataset dimension: " << dimension << ", elements: " << data.size() << std::endl;

    int d = 16;
    int nb = 10000;
    // 查询数量
    int nq = 10;
    // 最近邻数量
    int k = 4;
    int M = 16;
    size_t ef_construction = 200;  // Controls index search speed/build speed tradeoff

    // 生成示例数据
    std::vector<float> xb(nb * d);
    std::vector<float> xq(nq * d);

    // 填充数据集和查询数据
    for (int i = 0; i < nb; ++i) {
        for (int j = 0; j < d; ++j) {
            xb[i * d + j] = drand48();
        }
        xb[i * d] += i / 1000.0;
    }

    for (int i = 0; i < nq; ++i) {
        for (int j = 0; j < d; ++j) {
            xq[i * d + j] = drand48();
        }
        xq[i * d] += i / 1000.0;
    }

    // 创建 HNSW 索引
    faiss::IndexHNSWFlat index(d, M);
    index.hnsw.efConstruction = ef_construction;
    index.hnsw.efSearch = k;

    // 添加数据到索引
    index.add(nb, xb.data());

    // 查询索引
    std::vector<faiss::idx_t> I(nq * k);
    std::vector<float> D(nq * k);
    index.search(nq, xq.data(), k, D.data(), I.data());

    // 打印结果
    std::cout << "查询结果索引:" << std::endl;
    for (int i = 0; i < nq; ++i) {
        for (int j = 0; j < k; ++j) {
            std::cout << I[i * k + j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "查询结果距离:" << std::endl;
    for (int i = 0; i < nq; ++i) {
        for (int j = 0; j < k; ++j) {
            std::cout << D[i * k + j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}