
#include "mgw-source/filters/http/mgw_websocket/websocket_decoder.h"
#include "envoy/runtime/runtime.h"

namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{


WebSocketFrameType WebSocketDecoder::getFrameType(const Buffer::Instance& buffer){
  for(const Buffer::RawSlice& slice : buffer.getRawSlices()){
    const unsigned char* buffer_char = reinterpret_cast<const unsigned char*>(slice.mem_);
    unsigned char frame_opcode = buffer_char[0] & 0x0F;
    ENVOY_LOG(trace, "WebSocket frame opcode: {}", frame_opcode);
    // Data frames
    if (frame_opcode == 0x0) return WebSocketFrameType::CONTINUATION_FRAME;
    if (frame_opcode == 0x1) return WebSocketFrameType::TEXT_FRAME;
    if (frame_opcode == 0x2) return WebSocketFrameType::BINARY_FRAME;
    // Reserved non-control frames
    if (frame_opcode == 0x3 || frame_opcode == 0x4 || frame_opcode == 0x5 || 
        frame_opcode == 0x6 || frame_opcode == 0x7) return WebSocketFrameType::RESERVED_NON_CONTROL_FRAME;
    // Control frames
    if (frame_opcode == 0x8) return WebSocketFrameType::CLOSING_FRAME;
    if (frame_opcode == 0x9) return WebSocketFrameType::PING_FRAME;
    if (frame_opcode == 0xA) return WebSocketFrameType::PONG_FRAME;
    // Reserved control frames
    if (frame_opcode == 0xB || frame_opcode == 0xC || frame_opcode == 0xD || 
        frame_opcode == 0xE || frame_opcode == 0xF) return WebSocketFrameType::RESERVED_CONTROL_FRAME;
        
    }
    // Unreachable return
    return WebSocketFrameType::CONTINUATION_FRAME;
};

WebSocketFrameCategory WebSocketDecoder::getFrameCategory(const Buffer::Instance& buffer){
    WebSocketFrameType websocketFrameType = getFrameType(buffer);
    if(websocketFrameType == WebSocketFrameType::BINARY_FRAME || websocketFrameType == WebSocketFrameType::TEXT_FRAME || 
    websocketFrameType == WebSocketFrameType::CONTINUATION_FRAME){
        return WebSocketFrameCategory::DATA_FRAME;
    }else {
        return WebSocketFrameCategory::CONTROL_FRAME;
    }
}


} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy