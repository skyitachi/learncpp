set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)
find_package(absl CONFIG REQUIRED)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf REQUIRED)

message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)


if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")


set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

get_filename_component(common_proto "./proto/common.proto" ABSOLUTE)
get_filename_component(common_proto_path "${common_proto}" PATH)

message(STATUS, "proto_path: ${common_proto_path}")
message(STATUS, "bin dir: ${CMAKE_CURRENT_BINARY_DIR}")

set(common_proto_sources "${CMAKE_CURRENT_BINARY_DIR}/common.pb.cc")
set(common_proto_headers "${CMAKE_CURRENT_BINARY_DIR}/common.pb.h")
set(common_grpc_sources "${CMAKE_CURRENT_BINARY_DIR}/common.grpc.pb.cc")
set(common_grpc_headers "${CMAKE_CURRENT_BINARY_DIR}/common.grpc.pb.h")

add_custom_command(
	OUTPUT "${common_proto_sources}" "${common_proto_headers}" "${common_grpc_sources}" "${common_grpc_headers}"
	COMMAND ${_PROTOBUF_PROTOC}
	ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
		--cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
		-I "${common_proto_path}"
		--plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
		"${hw_proto}"
	DEPENDS "${hw_proto}"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_library(common_grpc_proto
	${common_grpc_sources}
	${common_grpc_headers}
	${common_proto_sources}
	${common_proto_headers}
)

target_link_libraries(common_grpc_proto
	${_REFLECTION}
	${_GRPC_GRPCPP}
	${_PROTOBUF_LIBPROTOBUF}
)

add_executable(faiss_server main.cpp)
# target_link_libraries(faiss_server faiss)
target_link_libraries(faiss_server
    common_grpc_proto
	faiss
)


add_executable(faiss_client client.cpp)

target_link_libraries(faiss_client
    common_grpc_proto
	faiss
)