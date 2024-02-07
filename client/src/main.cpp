#include <yandex/cloud/ai/stt/v2/stt_service.grpc.pb.h>
#include <yandex/cloud/ai/stt/v2/stt_service.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/client_context.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/server_builder.h>

#include <iostream>
#include <memory>
#include <string>

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include <grpcpp/grpcpp.h>

//ABSL_FLAG(std::string, target, "localhost:50051", "Server address");

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using namespace yandex::cloud::ai::stt::v2;
class GreeterClient {
public:
    GreeterClient(std::shared_ptr<Channel> channel)
        : stub_(SttService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string SendReq(const std::string& user) {
        auto a = SttService::Stub(stub_);
        ::SpeechRecognitionResult resp;
        ::yandex::cloud::ai::stt::v2::LongRunningRecognitionRequest req;
        // Data we are sending to the server.
        yandex::cloud::ai::stt::v2::RecognitionSpec request;
        request.set_language_code("ru_RU");
        request.set_profanity_filter(true);
        request.set_model("general");
        request.set_partial_results(true);
        request.set_audio_encoding(RecognitionSpec_AudioEncoding::RecognitionSpec_AudioEncoding_LINEAR16_PCM);
        request.set_sample_rate_hertz(8000);

        auto strconf = ::yandex::cloud::ai::stt::v2::RecognitionConfig();
        strconf.CopyFrom(request);
        auto msg = StreamingRecognitionRequest();
        msg.set_allocated_config(&strconf);
        ::grpc::ClientContext p;
        p.GetServerInitialMetadata();
        void LongRunningRecognize(::grpc::ClientContext * context, const ::yandex::cloud::ai::stt::v2::LongRunningRecognitionRequest* request, ::yandex::cloud::operation::Operation* response, std::function<void(::grpc::Status)>);

        // Container for the data we expect from the server.
        //        HelloReply reply;
        //
        //        // Context for the client. It could be used to convey extra information to
        //        // the server and/or tweak certain RPC behaviors.
        //        ClientContext context;
        //
        //        // The actual RPC.
        //        Status status = stub_->
        //
        //        // Act upon its status.
        //        if (status.ok()) {
        //            return reply.message();
        //        } else {
        //            std::cout << status.error_code() << ": " << status.error_message()
        //                      << std::endl;
        //            return "RPC failed";
        //        }
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
    absl::ParseCommandLine(argc, argv);
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint specified by
    // the argument "--target=" which is the only expected argument.
    std::string target_str = absl::GetFlag(FLAGS_target);
    // We indicate that the channel isn't authenticated (use of
    // InsecureChannelCredentials()).
    GreeterClient greeter(
        grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    std::string user("world");
    std::string reply = greeter.SayHello(user);
    std::cout << "Greeter received: " << reply << std::endl;

    return 0;
}