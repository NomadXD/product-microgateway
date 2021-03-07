
#include "mgw-source/filters/http/mgw_websocket/websocket_decoder.h"
#include "envoy/runtime/runtime.h"

namespace Envoy{
namespace Extensions{
namespace HttpFilters{
namespace MgwWebSocket{


/**

      0                   1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-------+-+-------------+-------------------------------+
     |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
     |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
     |N|V|V|V|       |S|             |   (if payload len==126/127)   |
     | |1|2|3|       |K|             |                               |
     +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
     |     Extended payload length continued, if payload len == 127  |
     + - - - - - - - - - - - - - - - +-------------------------------+
     |                               |Masking-key, if MASK set to 1  |
     +-------------------------------+-------------------------------+
     | Masking-key (continued)       |          Payload Data         |
     +-------------------------------- - - - - - - - - - - - - - - - +
     :                     Payload Data continued ...                :
     + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
     |                     Payload Data continued ...                |
     +---------------------------------------------------------------+

*/

WebSocketFrameCategory WebSocketDecoder::getFrameCategory(const Buffer::Instance& buffer){
    for(const Buffer::RawSlice& slice : buffer.getRawSlices()){
      const unsigned char* buffer_char = reinterpret_cast<const unsigned char*>(slice.mem_);
      unsigned char frame_opcode = buffer_char[0] & 0x0F;
      ENVOY_LOG(trace, "WebSocket frame opcode: {}", frame_opcode);
      // Data frames
      if(frame_opcode == 0x0 || frame_opcode == 0x1 || frame_opcode == 0x2 || frame_opcode == 0x3 || 
        frame_opcode == 0x4 || frame_opcode == 0x5 || frame_opcode == 0x6 || frame_opcode == 0x7){
          return WebSocketFrameCategory::DATA_FRAME;
      }else{
          return WebSocketFrameCategory::CONTROL_FRAME;
      }
    }
    return WebSocketFrameCategory::CONTROL_FRAME;

};


} // namespace MgwWebSocket
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy