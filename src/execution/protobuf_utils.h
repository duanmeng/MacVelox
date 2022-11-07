#ifndef MACVELOX_PROTOBUF_UTILS_H
#define MACVELOX_PROTOBUF_UTILS_H


#include <cstdint>
#include <google/protobuf/message.h>

namespace petrel::execution {

bool ParseProtobuf(
        const uint8_t* buf,
        int bufLen,
        google::protobuf::Message* msg);
}

#endif //MACVELOX_PROTOBUF_UTILS_H
