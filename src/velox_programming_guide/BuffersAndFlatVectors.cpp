#include "velox/buffer/Buffer.h" // AlignedBuffer
#include "velox/common/base/Nulls.h" // bits
#include "velox/common/memory/Memory.h" // MemoryPool, LOG
#include "velox/vector/BaseVector.h" // allocateNulls
#include "velox/vector/FlatVector.h" // FlatVector

#include <numeric>

using namespace ::facebook::velox;

DECLARE_bool(velox_exception_user_stacktrace_enabled);
DECLARE_bool(velox_exception_system_stacktrace_enabled);

int main() {
  auto pool = memory::getDefaultMemoryPool();
  auto values = AlignedBuffer::allocate<int64_t>(100, pool.get());
  auto nulls = AlignedBuffer::allocate<bool>(100, pool.get());

  LOG(INFO) << values->size() << ", " << values->capacity(); // 800, 928
  LOG(INFO) << nulls->size() << ", " << nulls->capacity(); // 13, 32

  values->setSize(900);
  nulls->setSize(20);

  try {
    values->setSize(1'000);
  } catch (VeloxRuntimeError e) {
    LOG(ERROR) << e.what();
    LOG(INFO) << values->size() << ", " << values->capacity(); // 900, 928
    LOG(INFO) << nulls->size() << ", " << nulls->capacity(); // 20, 32
  }

  auto* rawValues = values->as<int64_t>();
  LOG(INFO) << rawValues[5];

  values = AlignedBuffer::allocate<int64_t>(100, pool.get(), 25);
  rawValues = values->as<int64_t>();
  LOG(INFO) << rawValues[5];

  {
    auto* rawValues = values->asMutable<int64_t>();
    for (auto i = 0; i < 100; ++i) {
      rawValues[i] = i;
      LOG(INFO) << i << ": " << rawValues[i];
    }
    std::iota(rawValues, rawValues + 100, 0);
    for (auto i = 0; i < 10; ++i) {
      LOG(INFO) << i << ": " << rawValues[i];
    }
  }

  {
    // output garbage value for it's not init
    auto* rawNulls = nulls->as<uint64_t>();
    LOG(INFO) << std::boolalpha << bits::isBitNull(rawNulls, 5);
  }

  {
    auto nulls = AlignedBuffer::allocate<bool>(100, pool.get(), bits::kNull);
    auto* rawNulls = nulls->as<uint64_t>();
    LOG(INFO) << std::boolalpha << bits::isBitNull(rawNulls, 5);
  }

  {
    // use helper functions
    // Allocate nulls buffer to fit 100 null flags and initialize these to
    // bits::kNotNull. auto nulls = allocateNulls(100, pool.get()); Allocate
    // nulls buffer to fit 100 null flags and initialize these to bits::kNull.
    auto nulls = allocateNulls(100, pool.get(), bits::kNull);
    auto* rawNulls = nulls->asMutable<uint64_t>();

    for (auto i = 0; i < 100; ++i) {
      bits::setNull(rawNulls, i, i % 2 == 0);
    }

    for (auto i = 0; i < 10; ++i) {
      LOG(INFO) << i << ": " << std::boolalpha << bits::isBitNull(rawNulls, i);
    }

    LOG(INFO) << printNulls(nulls, 10);
    LOG(INFO) << printNulls(nulls);

    auto vector = std::make_shared<FlatVector<int64_t>>(
        pool.get(), BIGINT(), nulls, 100, values, std::vector<BufferPtr>{});
    LOG(INFO) << vector->toString();

    auto nonNullVector = std::make_shared<FlatVector<int64_t>>(
        pool.get(), BIGINT(), nullptr, 100, values, std::vector<BufferPtr>{});

    LOG(INFO) << nonNullVector->toString();
    LOG(INFO) << vector->size();
    LOG(INFO) << vector->type()->toString();
    LOG(INFO) << std::boolalpha << vector->isNullAt(5);
    LOG(INFO) << vector->valueAt(5);
    // Notice that the values buffer has a value for all positions even the ones
    // that are null. However, the value for null positions cannot be trusted.
    // It can be any value.
    LOG(INFO) << std::boolalpha << vector->isNullAt(6);
    LOG(INFO) << vector->valueAt(6);
  }

  {
    auto nulls = allocateNulls(100, pool.get(), bits::kNull);
    auto allNullVector = std::make_shared<FlatVector<int64_t>>(
        pool.get(), BIGINT(), nulls, 100, nullptr, std::vector<BufferPtr>{});

    LOG(INFO) << allNullVector->toString();
  }

  return 0;
}
