#include "arrow_memory_pool.h"

namespace petrel::memory {

std::shared_ptr<arrow::MemoryPool> AsWrappedArrowMemoryPool(
        petrel::memory::MemoryAllocator* allocator) {
    return std::make_shared<WrappedArrowMemoryPool>(allocator);
}

std::shared_ptr<arrow::MemoryPool> GetDefaultWrappedArrowMemoryPool() {
    static auto static_pool =
            AsWrappedArrowMemoryPool(petrel::memory::DefaultMemoryAllocator().get());
    return static_pool;
}

} // namespace petrel::memory