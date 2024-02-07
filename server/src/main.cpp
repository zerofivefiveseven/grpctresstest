#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"
#include <grpc/grpc.h>
#include <grpcpp/client_context.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/server_builder.h>
#include <iostream>
#include <memory>
#include <string>
#include <yandex/cloud/ai/stt/v2/stt_service.grpc.pb.h>
#include <yandex/cloud/ai/stt/v2/stt_service.pb.h>

//ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using namespace yandex::cloud::ai::stt::v2;

//template<class REQ, class RESP>
//class Tm_RecognizerData {
//    using Tm_Client = grpc::ClientAsyncReaderWriterInterface<REQ, RESP>;
//};
//
//template
//class Tm_RecognizerData<StreamingRecognitionRequest, StreamingRecognitionResponse>;
//// Logic and data behind the server's behavior.

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

//class GreeterServiceImpl final : public Greeter::Service {
//    Status SayHello(ServerContext *context, const HelloRequest *request,
//                    HelloReply *reply) override {
//        std::string prefix("Hello ");
//        reply->set_message(prefix + request->name());
//        return Status::OK;
//    }
//};

class MyService : public yandex::cloud::ai::stt::v2::SttService::Service {
    ::grpc::Status StreamingRecognize(::grpc::ServerContext* context,
                                      ::grpc::ServerReaderWriter<::yandex::cloud::ai::stt::v2::StreamingRecognitionResponse,
                                                                 ::yandex::cloud::ai::stt::v2::StreamingRecognitionRequest>* stream) override {
        std::cout << "StreamingRecognize called" << std::endl;
        stream->SendInitialMetadata();

        auto res = ::yandex::cloud::ai::stt::v2::StreamingRecognitionResponse();
        return ::grpc::Status::OK;
    }

public:
};

void RunServer(uint16_t port) {
    std::string server_address = absl::StrFormat("127.0.0.1:%d", port);
    MyService service;

    grpc::EnableDefaultHealthCheckService(true);
    //    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shut down. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
    //    absl::ParseCommandLine(argc, argv);
    //    uint_fast16_t a= ;
    RunServer(50051);
    return 0;
}
