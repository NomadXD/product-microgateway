#pragma once

#include "mgw-api/extensions/filters/http/mgw_websocket/v3/mgw_websocket.pb.h"
#include "mgw-api/extensions/filters/http/mgw_websocket/v3/mgw_websocket.pb.validate.h"

#include "extensions/filters/http/common/factory_base.h"
#include "extensions/filters/http/well_known_names.h"

namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{

class MgwWebSocketFilterConfig : public Common::FactoryBase<
                                    envoy::extensions::filters::http::mgw_websocket::v3::RateLimit> {

public:
    MgwWebSocketFilterConfig() : FactoryBase("envoy.filters.http.mgw_websocket") {}

private:
    Http::FilterFactoryCb createFilterFactoryFromProtoTyped(
        const envoy::extensions::filters::http::mgw_websocket::v3::RateLimit& proto_config,
        const std::string& stats_prefix, 
        Server::Configuration::FactoryContext& context
    ) override;
                                    
                                    
                                    
};

} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy

