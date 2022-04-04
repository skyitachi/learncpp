#include <iostream>
#include <faiss/IndexFlat.h>
#include <string>
#include <grpc/grpc.h>

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "common.grpc.pb.h"
#include "common.pb.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>

using idx_t = faiss::Index::idx_t;

using grpc::Status;
using grpc::Server;
using grpc::ServerContext;
using grpc::ServerBuilder;
class FaissIndexImpl final: public FaissIndex::Service {
public:
	explicit FaissIndexImpl(faiss::IndexFlatL2 *index) {
		index_flat_l2_ = index;
		
	}

	Status Search(ServerContext* context, const SearchRequest* req, SearchResponse* res) override {
		std::cout << "k is: " << req->k() << std::endl;
		res->set_msg("hello world");
		res->set_code(1);
		res->add_distance(1.0);
		res->add_labels(1);
		return Status::OK;
	} 

private:
	faiss::IndexFlatL2 *index_flat_l2_;
};

void start_grpc_server(faiss::IndexFlatL2 *index) {
	std::string server_address("0.0.0.0:10001");
	FaissIndexImpl service(index);
	
	grpc::reflection::InitProtoReflectionServerBuilderPlugin();

	ServerBuilder builder;

	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

	builder.RegisterService(&service);

	std::unique_ptr<Server> server(builder.BuildAndStart());

	std::cout << "Server Listening on" << server_address << std::endl;
	server->Wait();

	
}
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
	start_grpc_server(&index);
	return 0;

}