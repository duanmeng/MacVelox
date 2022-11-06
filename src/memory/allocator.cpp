#include "allocator.h"

namespace petrel::memory {

bool petrel::memory::ListenableMemoryAllocator::Allocate(
        int64_t size,
        void** out) {
    listener_->AllocationChanged(size);
    bool succeed = delegated_->Allocate(size, out);
    if (!succeed) {
        listener_->AllocationChanged(-size);
    }
    if (succeed) {
        bytes_ += size;
    }
    return succeed;
}

bool petrel::memory::ListenableMemoryAllocator::AllocateZeroFilled(
        int64_t nmemb,
        int64_t size,
        void** out) {
    listener_->AllocationChanged(size * nmemb);
    bool succeed = delegated_->AllocateZeroFilled(nmemb, size, out);
    if (!succeed) {
        listener_->AllocationChanged(-size * nmemb);
    }
    if (succeed) {
        bytes_ += size * nmemb;
    }
    return succeed;
}

bool petrel::memory::ListenableMemoryAllocator::AllocateAligned(
        uint16_t alignment,
        int64_t size,
        void** out) {
    listener_->AllocationChanged(size);
    bool succeed = delegated_->AllocateAligned(alignment, size, out);
    if (!succeed) {
        listener_->AllocationChanged(-size);
    }
    if (succeed) {
        bytes_ += size;
    }
    return succeed;
}

bool petrel::memory::ListenableMemoryAllocator::Reallocate(
        void* p,
        int64_t size,
        int64_t new_size,
        void** out) {
    int64_t diff = new_size - size;
    listener_->AllocationChanged(diff);
    bool succeed = delegated_->Reallocate(p, size, new_size, out);
    if (!succeed) {
        listener_->AllocationChanged(-diff);
    }
    if (succeed) {
        bytes_ += diff;
    }
    return succeed;
}

bool petrel::memory::ListenableMemoryAllocator::ReallocateAligned(
        void* p,
        uint16_t alignment,
        int64_t size,
        int64_t new_size,
        void** out) {
    int64_t diff = new_size - size;
    listener_->AllocationChanged(diff);
    bool succeed =
            delegated_->ReallocateAligned(p, alignment, size, new_size, out);
    if (!succeed) {
        listener_->AllocationChanged(-diff);
    }
    if (succeed) {
        bytes_ += diff;
    }
    return succeed;
}

bool petrel::memory::ListenableMemoryAllocator::Free(void* p, int64_t size) {
    listener_->AllocationChanged(-size);
    bool succeed = delegated_->Free(p, size);
    if (!succeed) {
        listener_->AllocationChanged(size);
    }
    if (succeed) {
        bytes_ -= size;
    }
    return succeed;
}

int64_t petrel::memory::ListenableMemoryAllocator::GetBytes() {
    return bytes_;
}

bool petrel::memory::StdMemoryAllocator::Allocate(int64_t size, void** out) {
    *out = std::malloc(size);
    bytes_ += size;
    return true;
}

bool petrel::memory::StdMemoryAllocator::AllocateZeroFilled(
        int64_t nmemb,
        int64_t size,
        void** out) {
    *out = std::calloc(nmemb, size);
    bytes_ += size;
    return true;
}

bool petrel::memory::StdMemoryAllocator::AllocateAligned(
        uint16_t alignment,
        int64_t size,
        void** out) {
    *out = aligned_alloc(alignment, size);
    bytes_ += size;
    return true;
}

bool petrel::memory::StdMemoryAllocator::Reallocate(
        void* p,
        int64_t size,
        int64_t new_size,
        void** out) {
    *out = std::realloc(p, new_size);
    bytes_ += (new_size - size);
    return true;
}

bool petrel::memory::StdMemoryAllocator::ReallocateAligned(
        void* p,
        uint16_t alignment,
        int64_t size,
        int64_t new_size,
        void** out) {
    if (new_size <= 0) {
        return false;
    }
    void* reallocated_p = std::realloc(p, new_size);
    if (!reallocated_p) {
        return false;
    }
    memcpy(reallocated_p, p, std::min(size, new_size));
    bytes_ += (new_size - size);
    return true;
}

bool petrel::memory::StdMemoryAllocator::Free(void* p, int64_t size) {
    std::free(p);
    bytes_ -= size;
    return true;
}

int64_t petrel::memory::StdMemoryAllocator::GetBytes() {
    return bytes_;
}

arrow::Status petrel::memory::WrappedArrowMemoryPool::Allocate(
        int64_t size,
        uint8_t** out) {
    if (!allocator_->Allocate(size, reinterpret_cast<void**>(out))) {
        return arrow::Status::Invalid(
                "WrappedMemoryPool: Error allocating " + std::to_string(size) +
                " bytes");
    }
    return arrow::Status::OK();
}

arrow::Status petrel::memory::WrappedArrowMemoryPool::Reallocate(
        int64_t old_size,
        int64_t new_size,
        uint8_t** ptr) {
    if (!allocator_->Reallocate(
            *ptr, old_size, new_size, reinterpret_cast<void**>(ptr))) {
        return arrow::Status::Invalid(
                "WrappedMemoryPool: Error reallocating " + std::to_string(new_size) +
                " bytes");
    }
    return arrow::Status::OK();
}

void petrel::memory::WrappedArrowMemoryPool::Free(
        uint8_t* buffer,
        int64_t size) {
    allocator_->Free(buffer, size);
}

int64_t petrel::memory::WrappedArrowMemoryPool::bytes_allocated() const {
    // fixme use self accountant
    return allocator_->GetBytes();
}

std::string petrel::memory::WrappedArrowMemoryPool::backend_name() const {
    return "petrel allocator";
}

[[maybe_unused]] std::shared_ptr<petrel::memory::MemoryAllocator>
DefaultMemoryAllocator() {
    static std::shared_ptr<MemoryAllocator> alloc =
            std::make_shared<StdMemoryAllocator>();
    return alloc;
}

} // namespace petrel::memory