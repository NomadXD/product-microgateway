#include "mgw-source/filters/http/mgw_websocket/config.h"

#include <chrono>
#include <string>

#include "mgw-api/extensions/filters/http/mgw_websocket/v3/mgw_websocket.pb.h"
#include "mgw-api/extensions/filters/http/mgw_websocket/v3/mgw_websocket.pb.validate.h"

#include "mgw-source/filters/http/mgw_websocket/ratelimit_impl.h"
#include "mgw-source/filters/http/mgw_websocket/mgw_websocket.h"

#include "envoy/registry/registry.h"

#include "common/protobuf/utility.h"

namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{

Http::FilterFactoryCb MgwWebSocketFilterConfig::createFilterFactoryFromProtoTyped(
    const envoy::extensions::filters::http::mgw_websocket::v3::RateLimit& proto_config,
    const std::string&, Server::Configuration::FactoryContext& context){

    FilterConfigSharedPtr filter_config(new FilterConfig(proto_config, context.localInfo(),
                                                       context.scope(), context.runtime(),
                                                       context.httpContext()));

    const std::chrono::milliseconds timeout =
      std::chrono::milliseconds(PROTOBUF_GET_MS_OR_DEFAULT(proto_config, timeout, 20));

    return [proto_config, &context, timeout,
        filter_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
        callbacks.addStreamFilter(std::make_shared<MgwWebSocketFilter>(filter_config, rateLimitClient(
                        context, proto_config.rate_limit_service().grpc_service(), timeout,
                        proto_config.rate_limit_service().transport_api_version())));
    };
};

// TODO (LahiruUdayanga) - createRouteSpecificFilterConfigTyped() for mgw_websocket

/**
 * Static registration for the mgw_websocket.
 */
REGISTER_FACTORY(MgwWebSocketFilterConfig,
                 Server::Configuration::NamedHttpFilterConfigFactory){"envoy.mgw_websocket"};


} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy

