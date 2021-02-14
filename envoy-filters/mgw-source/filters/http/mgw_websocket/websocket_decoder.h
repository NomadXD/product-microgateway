#pragma once

#include "envoy/runtime/runtime.h"

namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{

enum WebSocketFrameType {
  // RFC 6455 defined opcodes
  CONTINUATION_FRAME,
  TEXT_FRAME,
  BINARY_FRAME,
  CLOSING_FRAME,
  PING_FRAME,
  PONG_FRAME,
  // Reserved opcodes	for clients to use.
  RESERVED_NON_CONTROL_FRAME,
  RESERVED_CONTROL_FRAME,
};

enum WebSocketFrameCategory {
  DATA_FRAME,
  CONTROL_FRAME
};

class WebSocketDecoder :  public Logger::Loggable<Logger::Id::filter>{

public:
  static WebSocketFrameCategory getFrameCategory(const Buffer::Instance& buffer);

private:
  static WebSocketFrameType getFrameType(const Buffer::Instance& buffer);

};



} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy


