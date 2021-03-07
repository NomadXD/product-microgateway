#pragma once

#include "envoy/runtime/runtime.h"

namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{

enum WebSocketFrameCategory {
  DATA_FRAME,
  CONTROL_FRAME
};

class WebSocketDecoder :  public Logger::Loggable<Logger::Id::filter>{

public:
  static WebSocketFrameCategory getFrameCategory(const Buffer::Instance& buffer);
};



} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy


