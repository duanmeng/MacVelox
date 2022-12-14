#pragma once

#include <arrow/memory_pool.h>
#include <arrow/record_batch.h>
#include <arrow/type.h>
#include <arrow/type_fwd.h>
#include <folly/executors/IOThreadPoolExecutor.h>

#include "velox/common/file/FileSystems.h"
#include "velox/dwio/common/Options.h"
#include "velox/dwio/dwrf/reader/DwrfReader.h"
#include "velox/dwio/dwrf/writer/Writer.h"
#include "velox/vector/ComplexVector.h"

using namespace facebook::velox;

namespace velox {
namespace compute {

class DwrfDatasource {
public:
    DwrfDatasource(
            const std::string& file_path,
            std::shared_ptr<arrow::Schema> schema,
            memory::MemoryPool* pool)
            : file_path_(file_path), schema_(schema), pool_(pool) {}

    void Init(const std::unordered_map<std::string, std::string>& sparkConfs);
    std::shared_ptr<arrow::Schema> InspectSchema();
    void Write(const std::shared_ptr<arrow::RecordBatch>& rb);
    void Close();

private:
    std::string file_path_;
    std::string final_path_;
    int32_t count_ = 0;
    int64_t num_rbs_ = 0;
    std::shared_ptr<arrow::Schema> schema_;
    std::vector<RowVectorPtr> row_vecs_;
    std::shared_ptr<const facebook::velox::Type> type_;
    std::shared_ptr<facebook::velox::dwrf::Writer> writer_;
    memory::MemoryPool* pool_;
};

} // namespace compute
} // namespace velox
