#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "envoy/stream_info/stream_info.h"
#include "envoy/tracing/http_tracer.h"

namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{

enum class LimitStatus{
    // The request is not over limit.
    OK,
    // The rate limit service could not be queried.
    Error,
    // The request is over limit.
    OverLimit
};


/**
 * Async callbacks used during limit() calls.
 */
class RequestCallbacks{
public:
    virtual ~RequestCallbacks() = default;
    /**
    * Called when a limit request is complete. 
    */
    virtual void complete(LimitStatus status) PURE;

};


class Client{
public:
    virtual ~Client() = default;
    /**
    * Cancel an inflight limit request.
    */
    virtual void cancel() PURE;

    virtual void limit(RequestCallbacks& callbacks, const std::string& domain, envoy::config::core::v3::Metadata&& metadata_context,
                        Tracing::Span& parent_span, const StreamInfo::StreamInfo& stream_info) PURE;

};

using ClientPtr = std::unique_ptr<Client>;

} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy