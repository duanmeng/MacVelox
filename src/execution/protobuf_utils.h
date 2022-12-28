#pragma once

#include <cstdint>
#include <google/protobuf/message.h>

namespace petrel::execution {

bool ParseProtobuf(
        const uint8_t* buf,
        int bufLen,
        google::protobuf::Message* msg);
}
