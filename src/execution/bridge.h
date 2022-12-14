#pragma once

#include "arrow/c/bridge.h"
#include "arrow/util/iterator.h"

namespace petrel {
using ArrowArrayIterator = arrow::Iterator<std::shared_ptr<ArrowArray>>;
}
namespace arrow {
ARROW_EXPORT
Status ExportArrowArray(
        std::shared_ptr<arrow::Schema> schema,
        std::shared_ptr<petrel::ArrowArrayIterator> reader,
        struct ArrowArrayStream* out);
} // namespace arrow
