#include "mgw-source/filters/http/mgw_websocket/ratelimit_impl.h"
#include "envoy/server/filter_config.h"

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


GrpcClientImpl::~GrpcClientImpl() { ASSERT(!callbacks_); }

void GrpcClientImpl::cancel() {
  ASSERT(callbacks_ != nullptr);
  request_->cancel();
  callbacks_ = nullptr;
}

void GrpcClientImpl::createRequest(envoy::extensions::filters::http::mgw_websocket::v3::RateLimitRequest& request,
                            const std::string& domain) {
  request.set_domain(domain);
//   for (const envoy::extensions::filters::http::mgw_websocket::v3::RateLimitDescriptor& descriptor : descriptors) {
//     envoy::extensions::common::ratelimit::v3::RateLimitDescriptor* new_descriptor =
//         request.add_descriptors();
//     for (const Envoy::RateLimit::DescriptorEntry& entry : descriptor.entries_) {
//       envoy::extensions::common::ratelimit::v3::RateLimitDescriptor::Entry* new_entry =
//           new_descriptor->add_entries();
//       new_entry->set_key(entry.key_);
//       new_entry->set_value(entry.value_);
//     }
//     if (descriptor.limit_) {
//       envoy::extensions::common::ratelimit::v3::RateLimitDescriptor_RateLimitOverride* new_limit =
//           new_descriptor->mutable_limit();
//       new_limit->set_requests_per_unit(descriptor.limit_.value().requests_per_unit_);
//       new_limit->set_unit(descriptor.limit_.value().unit_);
//     }
//   }
}

void GrpcClientImpl::limit(RequestCallbacks& callbacks, const std::string& domain,
                           Tracing::Span& parent_span, const StreamInfo::StreamInfo&) {
  ASSERT(callbacks_ == nullptr);
  callbacks_ = &callbacks;

  envoy::extensions::filters::http::mgw_websocket::v3::RateLimitRequest request;
  createRequest(request, domain);

  request_ =
      async_client_->send(service_method_, request, *this, parent_span,
                          Http::AsyncClient::RequestOptions().setTimeout(timeout_),
                          transport_api_version_);
}

void GrpcClientImpl::onSuccess(
    std::unique_ptr<envoy::extensions::filters::http::mgw_websocket::v3::RateLimitResponse>&& response,
    Tracing::Span& span) {
  LimitStatus status = LimitStatus::OK;
  ASSERT(response->overall_code() != envoy::extensions::filters::http::mgw_websocket::v3::RateLimitResponse::UNKNOWN);
  if (response->overall_code() == envoy::extensions::filters::http::mgw_websocket::v3::RateLimitResponse::OVER_LIMIT) {
    status = LimitStatus::OverLimit;
    span.setTag(Constants::get().TraceStatus, Constants::get().TraceOverLimit);
  } else {
    span.setTag(Constants::get().TraceStatus, Constants::get().TraceOk);
  }

//   Http::ResponseHeaderMapPtr response_headers_to_add;
//   Http::RequestHeaderMapPtr request_headers_to_add;
//   if (!response->response_headers_to_add().empty()) {
//     response_headers_to_add = Http::ResponseHeaderMapImpl::create();
//     for (const auto& h : response->response_headers_to_add()) {
//       response_headers_to_add->addCopy(Http::LowerCaseString(h.key()), h.value());
//     }
//   }

//   if (!response->request_headers_to_add().empty()) {
//     request_headers_to_add = Http::RequestHeaderMapImpl::create();
//     for (const auto& h : response->request_headers_to_add()) {
//       request_headers_to_add->addCopy(Http::LowerCaseString(h.key()), h.value());
//     }
//   }

//   DescriptorStatusListPtr descriptor_statuses = std::make_unique<DescriptorStatusList>(
//       response->statuses().begin(), response->statuses().end());
//   callbacks_->complete(status, std::move(descriptor_statuses), std::move(response_headers_to_add),
//                        std::move(request_headers_to_add));
  callbacks_ = nullptr;
}

void GrpcClientImpl::onFailure(Grpc::Status::GrpcStatus status, const std::string&,
                               Tracing::Span&) {
  ASSERT(status != Grpc::Status::WellKnownGrpcStatus::Ok);
  callbacks_->complete(LimitStatus::Error);
  callbacks_ = nullptr;
}

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
  constexpr char V3[] = "envoy.extensions.filters.http.mgw_websocket.v3.RateLimitService.ShouldRateLimit";
  const auto* descriptor = Protobuf::DescriptorPool::generated_pool()->FindMethodByName(V3);
  ASSERT(descriptor != nullptr);
  return *descriptor;
}


} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
