#include "mgw-source/filters/http/mgw_websocket/mgw_websocket.h"

#include <string>
#include <vector>




namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{

void MgwWebSocketFilter::initiateCall(){
      client_ -> limit(*this, config_->domain(), callbacks_->activeSpan(), callbacks_-> streamInfo());

}

void MgwWebSocketFilter::complete(LimitStatus status){
  ENVOY_LOG(trace, "status :{}", status);

}

Http::FilterHeadersStatus MgwWebSocketFilter::decodeHeaders(Http::RequestHeaderMap&, bool) {
    ENVOY_LOG(trace, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>decodeHeaders called");
    // ProtobufWkt::Struct metadata;
    // auto& fields = *metadata.mutable_fields();
    // const std::string key{"key"};
    // const std::string value{"value"};
    // *fields[key]
    //      .mutable_string_value() = value;
    // const std::string filter{"sample.Decoder"};
    // callbacks_->streamInfo().setDynamicMetadata(filter, metadata);
    // //callbacks_->connection()->streamInfo()->setDynamicMetaData();
    // ENVOY_STREAM_LOG(trace, "metadata header:{}", *callbacks_);
    return Http::FilterHeadersStatus::Continue;
  //request_headers_ = &headers;
  //initiateCall(headers);
//   return (state_ == State::Calling || state_ == State::Responded)
//              ? Http::FilterHeadersStatus::StopIteration
//              : Http::FilterHeadersStatus::Continue;
}

Http::FilterDataStatus MgwWebSocketFilter::decodeData(Buffer::Instance&, bool) {
    initiateCall();
    ENVOY_LOG(trace, "decodeData called");
    ENVOY_LOG(trace, "domain :{}", config_->domain());
    // const ProtobufWkt::Value& meatadata_value = Envoy::Config::Metadata::metadataValue(callbacks_->streamInfo().dynamicMetadata(), 
    // "sample.Decoder","key");
    // ENVOY_LOG(trace, "metadata:{}", meatadata_value);
    //envoy::config::core::v3::Metadata metadata_context;
    //const auto& request_metadata = callbacks_->streamInfo().dynamicMetadata();
    
    //ENVOY_STREAM_LOG(trace, "metadata:{}", callbacks_);
//   if (state_ != State::Calling) {
//     return Http::FilterDataStatus::Continue;
//   }
  // If the request is too large, stop reading new data until the buffer drains.
  ENVOY_LOG(
      trace,
      "checking request: requestedServerName: {}, sourceIP: {}, directRemoteIP: {}, remoteIP: {},"
      "localAddress: {}, dynamicMetadata: {}",
      callbacks_->connection()->requestedServerName(),
      callbacks_->connection()->remoteAddress()->asString(),
      callbacks_->streamInfo().downstreamDirectRemoteAddress()->asString(),
      callbacks_->streamInfo().downstreamRemoteAddress()->asString(),
      callbacks_->streamInfo().downstreamLocalAddress()->asString(),
      callbacks_->streamInfo().dynamicMetadata().DebugString());
//   const auto& metadata = callbacks_->streamInfo().dynamicMetadata().filter_metadata();
//   const auto filter_it = metadata.find("sample.Decoder");
  //ENVOY_LOG(trace, "it:{}", filter_it->first);
  //ENVOY_LOG(trace, "it:{}", filter_it->second.DebugString());
  return Http::FilterDataStatus::Continue;
}

Http::FilterTrailersStatus MgwWebSocketFilter::decodeTrailers(Http::RequestTrailerMap&) {
  //ASSERT(state_ != State::Responded);
//   return state_ == State::Calling ? Http::FilterTrailersStatus::StopIteration
//                                   : Http::FilterTrailersStatus::Continue;
ENVOY_LOG(trace, "decodeTrailers called");
return Http::FilterTrailersStatus::Continue;
}

Http::FilterHeadersStatus MgwWebSocketFilter::encode100ContinueHeaders(Http::ResponseHeaderMap&) {
  ENVOY_LOG(trace, "encode100 called");
  return Http::FilterHeadersStatus::Continue;
}

Http::FilterHeadersStatus MgwWebSocketFilter::encodeHeaders(Http::ResponseHeaderMap&, bool) {
  //populateResponseHeaders(headers);
  ENVOY_LOG(trace, "encodeHeaders called");
  return Http::FilterHeadersStatus::Continue;
}

Http::FilterDataStatus MgwWebSocketFilter::encodeData(Buffer::Instance&, bool) {
    ENVOY_LOG(trace, "encodeData called");
  return Http::FilterDataStatus::Continue;
}

Http::FilterTrailersStatus MgwWebSocketFilter::encodeTrailers(Http::ResponseTrailerMap&) {
    ENVOY_LOG(trace, "encodeTrailers called");
  return Http::FilterTrailersStatus::Continue;
}

Http::FilterMetadataStatus MgwWebSocketFilter::encodeMetadata(Http::MetadataMap&) {
    ENVOY_LOG(trace, "encodeMetadata called");
  return Http::FilterMetadataStatus::Continue;
}

void MgwWebSocketFilter::setEncoderFilterCallbacks(Http::StreamEncoderFilterCallbacks&) {}

void MgwWebSocketFilter::onDestroy() {
//   if (state_ == State::Calling) {
//     state_ = State::Complete;
//     client_->cancel();
//   }



ENVOY_LOG(trace, "onDestroy called");
}

void MgwWebSocketFilter::setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) {
ENVOY_LOG(trace, "decoderCallbacks called");
  callbacks_ = &callbacks;
}





} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy