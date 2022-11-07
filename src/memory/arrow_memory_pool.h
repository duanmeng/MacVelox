#ifndef MACVELOX_ARROW_MEMORY_POOL_H
#define MACVELOX_ARROW_MEMORY_POOL_H

#include "allocator.h"

namespace petrel::memory {

std::shared_ptr<arrow::MemoryPool> AsWrappedArrowMemoryPool(
        MemoryAllocator* allocator);

std::shared_ptr<arrow::MemoryPool> GetDefaultWrappedArrowMemoryPool();

} // namespace petrel::memory

#endif //MACVELOX_ARROW_MEMORY_POOL_H
