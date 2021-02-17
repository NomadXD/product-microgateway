#pragma once

#include "mgw-source/filters/http/mgw_websocket/ratelimit.h"
#include "mgw-api/extensions/filters/http/mgw_websocket/v3/mgw_websocket.pb.h"
#include "envoy/local_info/local_info.h"
#include "envoy/stats/scope.h"
#include "envoy/runtime/runtime.h"
#include "envoy/http/context.h"
#include "envoy/http/filter.h"
#include <string>

namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{

/**
 * RateLimitType defines the type of rate limit applied.
 *  Default - Ratelimit applied in both directions.
 *  Downstream - Ratelimit applied for downstream -> upstream.
 *  Upstream - Ratelimit applied for upstream -> downstream.
 */
enum class RateLimitType {Default, Downstream, Upstream};

/**
 * RateLimitStatus defines the MgwWebsocket filter state. 
 *  UnderLimit - before rate limit
 *  OverLimit - after rate limit
 *  FailureModeAllowed - filter state when grpc connection to ratelimit service fails. 
 */
enum class RateLimitStatus {UnderLimit, OverLimit, FailureModeAllowed};

class FilterConfig {
public:
  FilterConfig(const envoy::extensions::filters::http::mgw_websocket::v3::RateLimit& config,
                const LocalInfo::LocalInfo& local_info, Stats::Scope& scope,
                Runtime::Loader& runtime, Http::Context& http_context)
                : domain_(config.domain()), ratelimit_type_(config.ratelimit_type().empty() ? stringToType("default") 
                        : stringToType(config.ratelimit_type())),
                local_info_(local_info), scope_(scope), runtime_(runtime),
                failure_mode_deny_(config.failure_mode_deny()),
                http_context_(http_context) {}

  const std::string& domain() const { return domain_; }
  const LocalInfo::LocalInfo& localInfo() const { return local_info_; }
  Runtime::Loader& runtime() { return runtime_; }
  Stats::Scope& scope() { return scope_; }
  RateLimitType rateLimitType() const { return ratelimit_type_;}
  bool failureModeAllow() const { return !failure_mode_deny_; }
  Http::Context& httpContext() { return http_context_; }

private:

  static RateLimitType stringToType(const std::string& rate_limit_type){
    if (rate_limit_type == "upstream"){
      return RateLimitType::Upstream;
    }else if (rate_limit_type == "downstream"){
      return RateLimitType::Downstream;
    }else {
      ASSERT(rate_limit_type == "default");
      return RateLimitType::Default;
    }
  }
    
  const std::string domain_;
  const RateLimitType ratelimit_type_;
  const LocalInfo::LocalInfo& local_info_;
  Stats::Scope& scope_;
  Runtime::Loader& runtime_;
  const bool failure_mode_deny_;
  Http::Context& http_context_;

};

using FilterConfigSharedPtr = std::shared_ptr<FilterConfig>;

class MgwWebSocketFilter : public Http::StreamFilter, 
                           public Logger::Loggable<Logger::Id::filter>, 
                           RequestCallbacks
                           {

public:
  MgwWebSocketFilter(FilterConfigSharedPtr config, ClientPtr&& client)
    : config_(config), client_(std::move(client)) {}

  // Http::StreamDecoderFilter
  Http::FilterHeadersStatus decodeHeaders(Http::RequestHeaderMap& headers,
                                          bool end_stream) override;
  Http::FilterDataStatus decodeData(Buffer::Instance& data, bool end_stream) override;
  Http::FilterTrailersStatus decodeTrailers(Http::RequestTrailerMap& trailers) override;
  void setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) override;

  // Http::StreamEncoderFilter
  Http::FilterHeadersStatus encode100ContinueHeaders(Http::ResponseHeaderMap& headers) override;
  Http::FilterHeadersStatus encodeHeaders(Http::ResponseHeaderMap& headers,
                                          bool end_stream) override;
  Http::FilterDataStatus encodeData(Buffer::Instance& data, bool end_stream) override;
  Http::FilterTrailersStatus encodeTrailers(Http::ResponseTrailerMap& trailers) override;
  Http::FilterMetadataStatus encodeMetadata(Http::MetadataMap&) override;
  void setEncoderFilterCallbacks(Http::StreamEncoderFilterCallbacks& callbacks) override;

  void onDestroy() override;
  // RequestCallbacks
  void complete(LimitStatus status) override;



private:
  FilterConfigSharedPtr config_;
  Http::StreamDecoderFilterCallbacks* decoder_callbacks_{};
  //Http::StreamEncoderFilterCallbacks* encoder_callbacks_{};
  ClientPtr client_;
  RateLimitStatus state_{RateLimitStatus::UnderLimit};
  //Http::ResponseHeaderMapPtr response_headers_to_add_;
  //Http::RequestHeaderMap* request_headers_{};
  //void publishMetaDataAsync(envoy::config::core::v3::Metadata&& metadata_context);
  void publishMetaDataAsync(const Buffer::Instance& buffer, const StreamInfo::StreamInfo& streamInfo);
};



} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy