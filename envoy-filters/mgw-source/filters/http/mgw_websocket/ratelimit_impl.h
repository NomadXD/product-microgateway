#pragma once

#include <chrono>
#include <string>
//#include "envoy/grpc/async_client.h"
#include "common/grpc/typed_async_client.h"
#include "mgw-api/extensions/filters/http/mgw_websocket/v3/mgw_websocket_rls.pb.h"
#include "mgw-source/filters/http/mgw_websocket/ratelimit.h"
#include "envoy/server/filter_config.h"

namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{

using RateLimitAsyncCallbacks = 
    Grpc::AsyncStreamCallbacks<envoy::extensions::filters::http::mgw_websocket::v3::RateLimitResponse>;

class GrpcClientImpl : public Client,
                       public Logger::Loggable<Logger::Id::config>,
                       public RateLimitAsyncCallbacks {
public:
  GrpcClientImpl(Grpc::RawAsyncClientPtr&& async_client,
                 const absl::optional<std::chrono::milliseconds>& timeout,
                 envoy::config::core::v3::ApiVersion transport_api_version);
  ~GrpcClientImpl() override;

  // MgwWebSocket::Client
  void cancel() override;
  void limit(RequestCallbacks& callbacks,const std::string& domain, envoy::config::core::v3::Metadata&& metadata_context) override;

  // Grpc::AsyncStreamCallbacks
  void onCreateInitialMetadata(Http::RequestHeaderMap&) override {}
  void onReceiveInitialMetadata(Http::ResponseHeaderMapPtr&&) override {}
  void onReceiveMessage(
    std::unique_ptr<envoy::extensions::filters::http::mgw_websocket::v3::RateLimitResponse>&& response) override;
  void onReceiveTrailingMetadata(Http::ResponseTrailerMapPtr&&) override {}
  void onRemoteClose(Grpc::Status::GrpcStatus status, const std::string& message) override;

private:

  void establishNewStream();
  //void handleFailure();

  Grpc::AsyncClient<envoy::extensions::filters::http::mgw_websocket::v3::RateLimitRequest,
                    envoy::extensions::filters::http::mgw_websocket::v3::RateLimitResponse>
      async_client_;
  Grpc::AsyncStream<envoy::extensions::filters::http::mgw_websocket::v3::RateLimitRequest> stream_{};
  absl::optional<std::chrono::milliseconds> timeout_;
  const Protobuf::MethodDescriptor& service_method_;
  const envoy::config::core::v3::ApiVersion transport_api_version_;
  envoy::extensions::filters::http::mgw_websocket::v3::RateLimitRequest message_;
  envoy::extensions::filters::http::mgw_websocket::v3::RateLimitResponse response_;
  static const Protobuf::MethodDescriptor& getMethodDescriptor();
  RequestCallbacks* callbacks_{};

};

ClientPtr rateLimitClient(Server::Configuration::FactoryContext& context,
                          const envoy::config::core::v3::GrpcService& grpc_service,
                          const std::chrono::milliseconds timeout,
                          envoy::config::core::v3::ApiVersion transport_api_version);



} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy