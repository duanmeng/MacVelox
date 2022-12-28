#pragma once

#include "allocator.h"

#include "velox/common/memory/Memory.h"
#include "substrait/plan.pb.h"
#include "substrait/type.pb.h"
#include "substrait/type_expressions.pb.h"

namespace petrel::memory {

constexpr uint16_t kNoAlignment = facebook::velox::memory::kNoAlignment;
constexpr int64_t kMaxMemory = facebook::velox::memory::kMaxMemory;

std::shared_ptr<facebook::velox::memory::MemoryPool> AsWrappedVeloxMemoryPool(
        petrel::memory::MemoryAllocator* allocator);

std::shared_ptr<facebook::velox::memory::MemoryPool>
GetDefaultWrappedVeloxMemoryPool();

} // namespace petrel::memory
