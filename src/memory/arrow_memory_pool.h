#pragma once

#include "allocator.h"

namespace petrel::memory {

std::shared_ptr<arrow::MemoryPool> AsWrappedArrowMemoryPool(
        MemoryAllocator* allocator);

std::shared_ptr<arrow::MemoryPool> GetDefaultWrappedArrowMemoryPool();

} // namespace petrel::memory
