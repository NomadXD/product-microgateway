#include "mgw-source/filters/http/mgw_websocket/mgw_websocket.h"
#include "mgw-source/filters/http/mgw_websocket/websocket_decoder.h"

#include <string>
#include <vector>




namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{

void MgwWebSocketFilter::publishMetaDataAsync(const Buffer::Instance& buffer, const StreamInfo::StreamInfo& streamInfo){
  envoy::config::core::v3::Metadata metadata_context;
  const auto& request_metadata = streamInfo.dynamicMetadata().filter_metadata();
  const auto& metadata_it = request_metadata.find("envoy.filters.http.ext_authz");
  if (metadata_it != request_metadata.end()) {
    (*metadata_context.mutable_filter_metadata())[metadata_it->first] = metadata_it->second;
  }
  ENVOY_LOG(trace, "initiate call called");
  ENVOY_LOG(trace, "buffer: {}", buffer.length());
  client_ -> limit(*this, config_->domain(), std::move(metadata_context));

}

void MgwWebSocketFilter::complete(LimitStatus status){
  ENVOY_LOG(trace, ">>>>> complete status :{}", status);
  if(status == LimitStatus::OK){
    state_ = RateLimitStatus::UnderLimit;
    ENVOY_LOG(debug, "OK");
  }else if (status == LimitStatus::OverLimit){
    state_ = RateLimitStatus::OverLimit;
    ENVOY_LOG(debug, "OverLimit");
  }else if (status == LimitStatus::Error){
    if(config_->failureModeAllow() == true){
      state_ = RateLimitStatus::FailureModeAllowed;
      ENVOY_LOG(debug, "failureModeAllowed");
    }else{
      state_ = RateLimitStatus::OverLimit;
      ENVOY_LOG(debug, "else");
    }
  }

}

Http::FilterHeadersStatus MgwWebSocketFilter::decodeHeaders(Http::RequestHeaderMap&, bool) {
    ENVOY_LOG(trace, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>decodeHeaders called");
    return Http::FilterHeadersStatus::Continue;

}

Http::FilterDataStatus MgwWebSocketFilter::decodeData(Buffer::Instance& data, bool) {

    if(data.length() > 0){
      WebSocketFrameCategory frame_category = WebSocketDecoder::getFrameCategory(data);
      if(frame_category == WebSocketFrameCategory::DATA_FRAME){
        // envoy::config::core::v3::Metadata metadata_context;
        // const auto& request_metadata = callbacks_->streamInfo().dynamicMetadata().filter_metadata();
        // const auto& metadata_it = request_metadata.find("envoy.filters.http.ext_authz");
        // if (metadata_it != request_metadata.end()) {
        //   (*metadata_context.mutable_filter_metadata())[metadata_it->first] = metadata_it->second;
        // }
        publishMetaDataAsync(data, decoder_callbacks_->streamInfo());
        if(config_->rateLimitType() == RateLimitType::Downstream || config_->rateLimitType() == RateLimitType::Default){
          if (state_ == RateLimitStatus::OverLimit){
          //callbacks_->sendLocalReply(Http::Code::TooManyRequests, "",nullptr,Grpc::Status::ResourceExhausted,"");
            return Http::FilterDataStatus::StopIterationNoBuffer;
          }else{
            return Http::FilterDataStatus::Continue;
          }
        }else{
          return Http::FilterDataStatus::Continue;
        }
      }else{
        ENVOY_LOG(trace, "control frame");
        return Http::FilterDataStatus::Continue;
      }
  // return state_== RateLimitStatus::OverLimit ? Http::FilterDataStatus::StopIterationNoBuffer 
  //                                             : Http::FilterDataStatus::Continue;
      
    }else {
      ENVOY_LOG(trace, "initial upgrade request");
      return Http::FilterDataStatus::Continue;
    } 

    
}

Http::FilterTrailersStatus MgwWebSocketFilter::decodeTrailers(Http::RequestTrailerMap&) {
ENVOY_LOG(trace, "decodeTrailers called >>>>>>>>>>>");
return state_== RateLimitStatus::OverLimit ? Http::FilterTrailersStatus::StopIteration 
                                              : Http::FilterTrailersStatus::Continue;
}

Http::FilterHeadersStatus MgwWebSocketFilter::encode100ContinueHeaders(Http::ResponseHeaderMap&) {
  ENVOY_LOG(trace, "encode100 called >>>>>>>>>>>");
  return Http::FilterHeadersStatus::Continue;
}

Http::FilterHeadersStatus MgwWebSocketFilter::encodeHeaders(Http::ResponseHeaderMap&, bool) {
  //populateResponseHeaders(headers);
  ENVOY_LOG(trace, "encodeHeaders called >>>>>>>>>>");
  return Http::FilterHeadersStatus::Continue;
}

Http::FilterDataStatus MgwWebSocketFilter::encodeData(Buffer::Instance& data, bool) {
  ENVOY_LOG(trace, "encodeData called >>>>>>>>>>>>>>>>>");
  if(data.length() > 0){
    ENVOY_LOG(trace, "106");
    WebSocketFrameCategory frame_category = WebSocketDecoder::getFrameCategory(data);
    ENVOY_LOG(trace, "108");
    if(frame_category == WebSocketFrameCategory::DATA_FRAME){
      ENVOY_LOG(trace, "110");
      publishMetaDataAsync(data, decoder_callbacks_->streamInfo());
          ENVOY_LOG(trace, "112");
      if(config_->rateLimitType() == RateLimitType::Upstream || config_->rateLimitType() == RateLimitType::Default){
            ENVOY_LOG(trace, "114");
        if (state_ == RateLimitStatus::OverLimit){
              ENVOY_LOG(trace, "116");
          return Http::FilterDataStatus::StopIterationNoBuffer;
        }else{
              ENVOY_LOG(trace, "120");
          return Http::FilterDataStatus::Continue;
        }
        }else{
              ENVOY_LOG(trace, "124");
          return Http::FilterDataStatus::Continue;
        }
      }else{
        ENVOY_LOG(trace, "control frame");
        return Http::FilterDataStatus::Continue;
      }
    
  }else {
    ENVOY_LOG(trace, "initial upgrade request");
    return Http::FilterDataStatus::Continue;
  }
  return Http::FilterDataStatus::Continue;
}

Http::FilterTrailersStatus MgwWebSocketFilter::encodeTrailers(Http::ResponseTrailerMap&) {
    ENVOY_LOG(trace, "encodeTrailers called >>>>>>>>>>>>>>>>");
  return Http::FilterTrailersStatus::Continue;
}

Http::FilterMetadataStatus MgwWebSocketFilter::encodeMetadata(Http::MetadataMap&) {
    ENVOY_LOG(trace, "encodeMetadata called >>>>>>>>>>>>>");
  return Http::FilterMetadataStatus::Continue;
}

void MgwWebSocketFilter::setEncoderFilterCallbacks(Http::StreamEncoderFilterCallbacks&) {}

void MgwWebSocketFilter::onDestroy() {
  client_->cancel();
  ENVOY_LOG(trace, "onDestroy called");
}

void MgwWebSocketFilter::setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) {
ENVOY_LOG(trace, "decoderCallbacks called");
  decoder_callbacks_ = &callbacks;
}





} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy