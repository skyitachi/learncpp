//
// Created by skyitachi on 24-7-19.
//

#include <hnswlib/hnswlib.h>
#include <iostream>
#include <cmath>
#include <vector>

int main() {
    int dim = 16;               // Dimension of the elements
    size_t max_elements = 10000;   // Maximum number of elements, should be known beforehand
    size_t M = 16;                 // Tightly connected with internal dimensionality of the data
    // strongly affects the memory consumption
    size_t ef_construction = 200;  // Controls index search speed/build speed tradeoff

    // Initing index
    hnswlib::L2Space space(dim);
    hnswlib::HierarchicalNSW<float>* alg_hnsw = new hnswlib::HierarchicalNSW<float>(&space,
                                                                                    max_elements,
                                                                                    M,
                                                                                    ef_construction);

    std::cout << "ef_: " << alg_hnsw->ef_ << std::endl;
    // Generate random data
    std::mt19937 rng;
    rng.seed(47);
    std::uniform_real_distribution<> distrib_real;
    float* data = new float[dim * max_elements];
    for (int i = 0; i < dim * max_elements; i++) {
        data[i] = distrib_real(rng);
    }

    // Add data to index
    for (int i = 0; i < max_elements; i++) {
        alg_hnsw->addPoint(data + i * dim, i);
    }


    auto cur_count = alg_hnsw->cur_element_count.load();

    auto max_level = std::max_element(alg_hnsw->element_levels_.begin(), alg_hnsw->element_levels_.begin() +cur_count);

    int cal_max_level = 0;
    for(auto level: alg_hnsw->element_levels_) {
        if (level > cal_max_level) {
            cal_max_level = level;
        }
    }

    std::cout << "cur_count: " << cur_count << ", max level: " << *max_level << ", data_size: " << alg_hnsw->data_size_ << std::endl;

    std::cout << "manual cal_max_level: " << cal_max_level << std::endl;

    std::cout << "expected levels: " << std::log(cur_count) / std::log(M) << std::endl;

    std::vector<int> list = {1, 2, 3, 4};

    auto max_element = std::max_element(list.begin(), list.end());

    std::cout << "max_element: " << *max_element << std::endl;

    auto result = alg_hnsw->searchKnn(data, 10);

    while (!result.empty()) {
        auto top = result.top();
        std::cout << "distance = " << top.first << ", label = " << top.second << std::endl;
        result.pop();
    }

}