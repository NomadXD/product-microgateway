#include "mgw-source/filters/http/mgw_websocket/mgw_websocket.h"
#include "common/common/logger.h"
#include "mgw-source/filters/http/mgw_websocket/websocket_decoder.h"
#include "ratelimit.h"

#include <bits/stdint-uintn.h>
#include <string>
#include <vector>

namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{

void MgwWebSocketFilter::publishMetaDataAsync(const Buffer::Instance& buffer, const StreamInfo::StreamInfo& streamInfo){
  envoy::config::core::v3::Metadata metadata_context{};
  // Read metadata added by ext_authz filter
  const auto& request_metadata = streamInfo.dynamicMetadata().filter_metadata();
  const auto& metadata_it = request_metadata.find("envoy.filters.http.ext_authz");
  if (metadata_it != request_metadata.end()) {
    (*metadata_context.mutable_filter_metadata())[metadata_it->first] = metadata_it->second;
  }
  // Create mgw_websocket metadata 
  ProtobufWkt::Struct mgw_websocket_metadata{};
  auto& fields = *mgw_websocket_metadata.mutable_fields();
  const std::string frame_length_key{"frame_length"};
  const uint64_t frame_length_value{buffer.length()};
  const std::string upstream_host_key{"upstream_host"};
  const std::string upstream_host_value{streamInfo.upstreamHost()->hostname()};
  const std::string remote_ip_key{"remote_ip"};
  const std::string remote_ip_value{streamInfo.upstreamHost()->address()->asString()};
  fields[frame_length_key].set_number_value(frame_length_value);
  *fields[upstream_host_key].mutable_string_value() = upstream_host_value;
  *fields[remote_ip_key].mutable_string_value() = remote_ip_value;
  (*metadata_context.mutable_filter_metadata())["envoy.filters.http.mgw_websocket"] = mgw_websocket_metadata;
  client_ -> limit(*this, config_->domain(), std::move(metadata_context));
}

void MgwWebSocketFilter::complete(LimitStatus status){
  ENVOY_LOG(trace, "complete status :{}", status);
  if(status == LimitStatus::OK){
    state_ = RateLimitStatus::UnderLimit;
    ENVOY_LOG(trace, "Filter state updated to : {}", LimitStatus::OK);
  }else if (status == LimitStatus::OverLimit){
    state_ = RateLimitStatus::OverLimit;
    ENVOY_LOG(trace, "Filter state updated to : {}", LimitStatus::OverLimit);
  }else if (status == LimitStatus::Error){
    ENVOY_LOG(trace, "Ratelimit client returned LimitStatus:Error");
    if(config_->failureModeAllow() == true){
      state_ = RateLimitStatus::FailureModeAllowed;
      ENVOY_LOG(trace, "Filter state updated to : {}", RateLimitStatus::FailureModeAllowed);
    }else{
      state_ = RateLimitStatus::OverLimit;
      ENVOY_LOG(trace, "Filter state updated to : {}", RateLimitStatus::OverLimit);
    }
  }

}

Http::FilterHeadersStatus MgwWebSocketFilter::decodeHeaders(Http::RequestHeaderMap&, bool) {
    ENVOY_LOG(trace, "mgw_websocket decodeHeaders called");
    ENVOY_LOG(debug, "demo debug message");
    return Http::FilterHeadersStatus::Continue;

}


Http::FilterDataStatus MgwWebSocketFilter::decodeData(Buffer::Instance& data, bool) {
  // To avoid initial upgrade request with data.length() = 0 and validate a 
  // valid websocket frame by the size. A websocket frame should atleast have
  // 3 bytes.  
  if(data.length() >= 3){
    WebSocketFrameCategory frame_category = WebSocketDecoder::getFrameCategory(data);
    if(frame_category == WebSocketFrameCategory::DATA_FRAME){
      ENVOY_LOG(trace, "WebSocket data frame : {} bytes", data.length());
      publishMetaDataAsync(data, decoder_callbacks_->streamInfo());
      if(config_->rateLimitType() == RateLimitType::Downstream || config_->rateLimitType() == RateLimitType::Default){
        if (state_ == RateLimitStatus::OverLimit){
        //callbacks_->sendLocalReply(Http::Code::TooManyRequests, "",nullptr,Grpc::Status::ResourceExhausted,"");
          ENVOY_LOG(info, "WebSocket connection throttled");
          return Http::FilterDataStatus::StopIterationNoBuffer;
        }else{
          return Http::FilterDataStatus::Continue;
        }
      }else{
        return Http::FilterDataStatus::Continue;
      }
    }else{
      ENVOY_LOG(trace, "WebSocket control frame: {} bytes", data.length());
      return Http::FilterDataStatus::Continue;
    }    
  }else {
    ENVOY_LOG(trace, "initial upgrade request");
    return Http::FilterDataStatus::Continue;
  } 

}

Http::FilterTrailersStatus MgwWebSocketFilter::decodeTrailers(Http::RequestTrailerMap&) {
  ENVOY_LOG(trace, "mgw_websocket decodeTrailers called");
  return state_== RateLimitStatus::OverLimit ? Http::FilterTrailersStatus::StopIteration 
                                              : Http::FilterTrailersStatus::Continue;
}

Http::FilterHeadersStatus MgwWebSocketFilter::encode100ContinueHeaders(Http::ResponseHeaderMap&) {
  ENVOY_LOG(trace, "mgw_websocket encode100 called");
  return Http::FilterHeadersStatus::Continue;
}

Http::FilterHeadersStatus MgwWebSocketFilter::encodeHeaders(Http::ResponseHeaderMap&, bool) {
  ENVOY_LOG(trace, "mgw_websocket encodeHeaders called");
  return Http::FilterHeadersStatus::Continue;
}

Http::FilterDataStatus MgwWebSocketFilter::encodeData(Buffer::Instance& data, bool) {
  // To avoid initial upgrade request with data.length() = 0 and validate a 
  // valid websocket frame by the size. A websocket frame should atleast have
  // 3 bytes.  
  if(data.length() > 0){
    WebSocketFrameCategory frame_category = WebSocketDecoder::getFrameCategory(data);
    if(frame_category == WebSocketFrameCategory::DATA_FRAME){
      ENVOY_LOG(trace, "WebSocket data frame : {} bytes", data.length());
      publishMetaDataAsync(data, decoder_callbacks_->streamInfo());
      if(config_->rateLimitType() == RateLimitType::Upstream || config_->rateLimitType() == RateLimitType::Default){
        if (state_ == RateLimitStatus::OverLimit){
          ENVOY_LOG(info, "WebSocket connection throttled");
          return Http::FilterDataStatus::StopIterationNoBuffer;
        }else{
          return Http::FilterDataStatus::Continue;
        }
        }else{
          return Http::FilterDataStatus::Continue;
        }
      }else{
        ENVOY_LOG(trace, "WebSocket control frame: {} bytes", data.length());
        return Http::FilterDataStatus::Continue;
      }
    
  }else {
    ENVOY_LOG(trace, "initial upgrade request");
    return Http::FilterDataStatus::Continue;
  }
  return Http::FilterDataStatus::Continue;
}

Http::FilterTrailersStatus MgwWebSocketFilter::encodeTrailers(Http::ResponseTrailerMap&) {
  ENVOY_LOG(trace, "mgw_websocket encodeTrailers called");
  return Http::FilterTrailersStatus::Continue;
}

Http::FilterMetadataStatus MgwWebSocketFilter::encodeMetadata(Http::MetadataMap&) {
  ENVOY_LOG(trace, "mgw_websocket encodeMetadata called");
  return Http::FilterMetadataStatus::Continue;
}

void MgwWebSocketFilter::setEncoderFilterCallbacks(Http::StreamEncoderFilterCallbacks&) {}

void MgwWebSocketFilter::onDestroy() {
  client_->cancel();
  ENVOY_LOG(trace, "mgw_websocket onDestroy() called");
}

void MgwWebSocketFilter::setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) {
  ENVOY_LOG(trace, "mgw_websocket decoderCallbacks called");
  decoder_callbacks_ = &callbacks;
}


} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy