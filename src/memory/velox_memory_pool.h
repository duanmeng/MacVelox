//
// Created by macduan on 2022/11/5.
//

#ifndef MACVELOX_VELOX_MEMORY_POOL_H
#define MACVELOX_VELOX_MEMORY_POOL_H

#include "velox/common/memory/Memory.h"
#include "substrait/plan.pb.h"
#include "substrait/type.pb.h"
#include "substrait/type_expressions.pb.h"

namespace petrel::memory {

constexpr uint16_t kNoAlignment = facebook::velox::memory::kNoAlignment;
constexpr int64_t kMaxMemory = facebook::velox::memory::kMaxMemory;

class velox_memory_pool {
};

} // namespace petrel::memory

#endif //MACVELOX_VELOX_MEMORY_POOL_H
