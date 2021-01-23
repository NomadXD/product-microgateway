#include "mgw-source/filters/http/mgw_websocket/ratelimit_impl.h"
#include "envoy/server/filter_config.h"
#include "mgw-api/extensions/filters/http/mgw_websocket/v3/mgw_websocket_rls.pb.h"
#include <string>

namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{

GrpcClientImpl::GrpcClientImpl(Grpc::RawAsyncClientPtr&& async_client,
                               const absl::optional<std::chrono::milliseconds>& timeout,
                               envoy::config::core::v3::ApiVersion transport_api_version)
    : async_client_(std::move(async_client)), timeout_(timeout),
      service_method_(
          getMethodDescriptor()),
      transport_api_version_(transport_api_version) {}


GrpcClientImpl::~GrpcClientImpl() {  }

void GrpcClientImpl::cancel() {
  ENVOY_LOG(trace, "Cancel");
  callbacks_ = nullptr;
  stream_->closeStream();
}


void GrpcClientImpl::limit(RequestCallbacks& callbacks,const std::string& domain, envoy::config::core::v3::Metadata&& metadata_context) {

  callbacks_= &callbacks;
  message_.set_domain(domain);
  auto metadata_ctx = message_.mutable_metadata_context();
  *metadata_ctx = std::move(metadata_context);
  if(stream_ != nullptr){
    ENVOY_LOG(trace, "gRPC bidi stream exist for service method: {}", service_method_.DebugString());
    stream_->sendMessage(message_, transport_api_version_,false);
    message_.Clear();
    ENVOY_LOG(trace, "message successfully sent through gRPC bidi stream with service method : {}", service_method_.DebugString());
  }else{
    ENVOY_LOG(trace, "initializing gRPC bidi stream for service method : {}", service_method_.DebugString());
    stream_ = async_client_->start(service_method_, *this, Http::AsyncClient::StreamOptions());
    stream_->sendMessage(message_, transport_api_version_,false);
    message_.Clear();
    ENVOY_LOG(trace, "message successfully sent through gRPC bidi stream with service method : {}", service_method_.DebugString());
  }
}

void GrpcClientImpl::onReceiveMessage(std::unique_ptr<envoy::extensions::filters::http::mgw_websocket::v3::RateLimitResponse>&& response) {
  ENVOY_LOG(trace, "onReceiveMessage invoked for gRPC bidi stream with service method : {}", service_method_.DebugString());
  if (response->overall_code() == envoy::extensions::filters::http::mgw_websocket::v3::RateLimitResponse::OVER_LIMIT) {
    callbacks_->complete(LimitStatus::OverLimit);
  } else {
    callbacks_->complete(LimitStatus::OK);
  }
};


void GrpcClientImpl::onRemoteClose(Grpc::Status::GrpcStatus status, const std::string& message) {
      stream_ = nullptr;
      callbacks_->complete(LimitStatus::Error);
      ENVOY_LOG(trace, "onRemoteClose : {} {}", status, message);
};

void GrpcClientImpl::establishNewStream(){
      ENVOY_LOG(trace, "Establishing new gRPC bidi stream for {}", service_method_.DebugString());
      if (stream_ == nullptr){
            ENVOY_LOG(trace, "before starting the stream");
        stream_ = async_client_->start(service_method_, *this, Http::AsyncClient::StreamOptions());
      }
      if (stream_ == nullptr){
        ENVOY_LOG(trace, "Unable to establish a new stream");
      }
};

ClientPtr rateLimitClient(Server::Configuration::FactoryContext& context,
                          const envoy::config::core::v3::GrpcService& grpc_service,
                          const std::chrono::milliseconds timeout,
                          envoy::config::core::v3::ApiVersion transport_api_version) {
  const auto async_client_factory =
      context.clusterManager().grpcAsyncClientManager().factoryForGrpcService(
          grpc_service, context.scope(), true);
  return std::make_unique<GrpcClientImpl>(
      async_client_factory->create(), timeout, transport_api_version);
}

const Protobuf::MethodDescriptor& GrpcClientImpl::getMethodDescriptor(){
  constexpr char V3[] = "envoy.extensions.filters.http.mgw_websocket.v3.RateLimitService.ShouldRateLimitStream";
  const auto* descriptor = Protobuf::DescriptorPool::generated_pool()->FindMethodByName(V3);
  ASSERT(descriptor != nullptr);
  return *descriptor;
}


} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
