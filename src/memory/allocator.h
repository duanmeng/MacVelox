#ifndef MACVELOX_ALLOCATOR_H
#define MACVELOX_ALLOCATOR_H


#include "arrow/memory_pool.h"
#include "arrow/util/memory.h"

namespace petrel::memory {

#include <atomic>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <utility>

class MemoryAllocator {
public:
    virtual ~MemoryAllocator() = default;
    virtual bool Allocate(int64_t size, void** out) = 0;
    virtual bool AllocateZeroFilled(int64_t nmemb, int64_t size, void** out) = 0;
    virtual bool
    AllocateAligned(uint16_t alignment, int64_t size, void** out) = 0;
    virtual bool
    Reallocate(void* p, int64_t size, int64_t new_size, void** out) = 0;
    virtual bool ReallocateAligned(
            void* p,
            uint16_t alignment,
            int64_t size,
            int64_t new_size,
            void** out) = 0;
    virtual bool Free(void* p, int64_t size) = 0;
    virtual int64_t GetBytes() = 0;
};

class AllocationListener {
public:
    virtual ~AllocationListener() = default;

    // Value of diff can be either positive or negative
    virtual void AllocationChanged(int64_t diff) = 0;

protected:
    AllocationListener() = default;
};

class ListenableMemoryAllocator : public MemoryAllocator {
public:
    explicit ListenableMemoryAllocator(
            MemoryAllocator* delegated,
            std::shared_ptr<AllocationListener> listener)
            : delegated_(delegated), listener_(std::move(listener)) {}

public:
    bool Allocate(int64_t size, void** out) override;

    bool AllocateZeroFilled(int64_t nmemb, int64_t size, void** out) override;

    bool AllocateAligned(uint16_t alignment, int64_t size, void** out) override;

    bool Reallocate(void* p, int64_t size, int64_t new_size, void** out) override;

    bool ReallocateAligned(
            void* p,
            uint16_t alignment,
            int64_t size,
            int64_t new_size,
            void** out) override;

    bool Free(void* p, int64_t size) override;

    int64_t GetBytes() override;

private:
    MemoryAllocator* delegated_;
    std::shared_ptr<AllocationListener> listener_;
    std::atomic_int64_t bytes_{0};
};

class StdMemoryAllocator : public MemoryAllocator {
public:
    bool Allocate(int64_t size, void** out) override;

    bool AllocateZeroFilled(int64_t nmemb, int64_t size, void** out) override;

    bool AllocateAligned(uint16_t alignment, int64_t size, void** out) override;

    bool Reallocate(void* p, int64_t size, int64_t new_size, void** out) override;

    bool ReallocateAligned(
            void* p,
            uint16_t alignment,
            int64_t size,
            int64_t new_size,
            void** out) override;

    bool Free(void* p, int64_t size) override;

    int64_t GetBytes() override;

private:
    std::atomic_int64_t bytes_{0};
};

// TODO aligned allocation
class WrappedArrowMemoryPool : public arrow::MemoryPool {
public:
    explicit WrappedArrowMemoryPool(MemoryAllocator* allocator)
            : allocator_(allocator) {}

    arrow::Status Allocate(int64_t size, uint8_t** out) override;

    arrow::Status Reallocate(int64_t old_size, int64_t new_size, uint8_t** ptr)
    override;

    void Free(uint8_t* buffer, int64_t size) override;

    int64_t bytes_allocated() const override;

    std::string backend_name() const override;

private:
    MemoryAllocator* allocator_;
};

[[maybe_unused]] std::shared_ptr<MemoryAllocator> DefaultMemoryAllocator();

} // namespace petrel::memory

#endif //MACVELOX_ALLOCATOR_H
