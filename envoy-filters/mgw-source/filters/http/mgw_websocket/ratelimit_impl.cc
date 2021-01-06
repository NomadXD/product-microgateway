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

// void GrpcClientImpl::cancel() {
//   ENVOY_LOG(trace, "Cancel");
//   //ASSERT(callbacks_ != nullptr);
//   // request_->cancel();
//   // callbacks_ = nullptr;
// }

// void GrpcClientImpl::createRequest(envoy::extensions::filters::http::mgw_websocket::v3::RateLimitRequest& request,
//                             const std::string& domain, envoy::config::core::v3::Metadata&& metadata_context) {
  
//   request.set_domain(domain);
//   //google::protobuf::Struct* pointer = &metadata;
//   ;
//   auto metadata_ctx = request.mutable_metadata_context();
//   *metadata_ctx = std::move(metadata_context);
// //   for (const envoy::extensions::filters::http::mgw_websocket::v3::RateLimitDescriptor& descriptor : descriptors) {
// //     envoy::extensions::common::ratelimit::v3::RateLimitDescriptor* new_descriptor =
// //         request.add_descriptors();
// //     for (const Envoy::RateLimit::DescriptorEntry& entry : descriptor.entries_) {
// //       envoy::extensions::common::ratelimit::v3::RateLimitDescriptor::Entry* new_entry =
// //           new_descriptor->add_entries();
// //       new_entry->set_key(entry.key_);
// //       new_entry->set_value(entry.value_);
// //     }
// //     if (descriptor.limit_) {
// //       envoy::extensions::common::ratelimit::v3::RateLimitDescriptor_RateLimitOverride* new_limit =
// //           new_descriptor->mutable_limit();
// //       new_limit->set_requests_per_unit(descriptor.limit_.value().requests_per_unit_);
// //       new_limit->set_unit(descriptor.limit_.value().unit_);
// //     }
// //   }
// }

void GrpcClientImpl::limit(RequestCallbacks& callbacks,const std::string& domain, envoy::config::core::v3::Metadata&& metadata_context) {

  ENVOY_LOG(trace, "inside limit");
  //createRequest(message, domain, std::move(metadata_context));
  callbacks_= &callbacks;
  ENVOY_LOG(trace, "before establish new stream");
  //establishNewStream();
  ENVOY_LOG(trace, "after establish string");
  message_.set_domain(domain);
  ENVOY_LOG(trace, "after set domain");
  auto metadata_ctx = message_.mutable_metadata_context();
  ENVOY_LOG(trace, "after metadata ctx");
  *metadata_ctx = std::move(metadata_context);
  ENVOY_LOG(trace,"after move");
  if(stream_ != nullptr){
        ENVOY_LOG(trace, "before send message");
        ENVOY_LOG(trace, transport_api_version_);
        stream_->sendMessage(message_, transport_api_version_,false);
        message_.Clear();
        ENVOY_LOG(trace, "after send message");
  }else{
        ENVOY_LOG(trace, "Error sending the message");
        stream_ = async_client_->start(service_method_, *this, Http::AsyncClient::StreamOptions());
        stream_->sendMessage(message_, transport_api_version_,false);
        message_.Clear();
  }
  
  ENVOY_LOG(trace, "after meesage clearedd");
  
}

// void GrpcClientImpl::onCreateInitialMetadata(Http::RequestHeaderMap&) {
//   ENVOY_LOG(trace, "onCreateInitialMetadata");
// };

// void GrpcClientImpl::onReceiveInitialMetadata(Http::ResponseHeaderMapPtr&&) {
//   ENVOY_LOG(trace, "onReceiveInitialMetadata");
// };

void GrpcClientImpl::onReceiveMessage(std::unique_ptr<envoy::extensions::filters::http::mgw_websocket::v3::RateLimitResponse>&& response) {
      ENVOY_LOG(trace, "onReceiveMessage");
      LimitStatus status = LimitStatus::OK;
      if (response->overall_code() == envoy::extensions::filters::http::mgw_websocket::v3::RateLimitResponse::OVER_LIMIT) {
    status = LimitStatus::OverLimit;
    
  } else {
          status = LimitStatus::OK;
          ENVOY_LOG(trace, "onReceiveMessage");

  }
};
// void GrpcClientImpl::onReceiveTrailingMetadata(Http::ResponseTrailerMapPtr&&) {
//     ENVOY_LOG(trace, "onReceiveTrailingMetadata");
// };
void GrpcClientImpl::onRemoteClose(Grpc::Status::GrpcStatus status, const std::string& message) {
      stream_ = nullptr;
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
  // TODO(ramaraochavali): register client to singleton when GrpcClientImpl supports concurrent
  // requests.
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
