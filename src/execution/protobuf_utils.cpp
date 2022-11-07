#include "protobuf_utils.h"

namespace petrel::execution {

inline bool ParseProtobuf(
        const uint8_t *buf,
        int bufLen,
        google::protobuf::Message *msg) {
    google::protobuf::io::ArrayInputStream buf_stream{buf, bufLen};
    return msg->ParseFromZeroCopyStream(&buf_stream);
}

} // namespace petrel::execution
