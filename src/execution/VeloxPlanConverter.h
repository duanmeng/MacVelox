#pragma once

#include "exec_backend.h"
#include "columnar_batch.h"
#include "velox_memory_pool.h"

#include <arrow/array/array_binary.h>
#include <arrow/array/array_primitive.h>
#include <arrow/array/data.h>
#include <arrow/array/util.h>
#include <arrow/record_batch.h>
#include <arrow/type_fwd.h>
#include "arrow/c/abi.h"
#include <folly/executors/IOThreadPoolExecutor.h>

#include "substrait/algebra.pb.h"
#include "substrait/capabilities.pb.h"
#include "substrait/extensions/extensions.pb.h"
#include "substrait/function.pb.h"
#include "substrait/parameterized_types.pb.h"
#include "substrait/plan.pb.h"
#include "substrait/type.pb.h"
#include "substrait/type_expressions.pb.h"
#include "utils/metrics.h"
#include "velox/common/file/FileSystems.h"
#include "velox/connectors/hive/FileHandle.h"
#include "velox/connectors/hive/HiveConnector.h"
#include "velox/connectors/hive/HiveConnectorSplit.h"
#include "velox/core/Expressions.h"
#include "velox/core/ITypedExpr.h"
#include "velox/core/PlanNode.h"
#include "velox/dwio/common/Options.h"
#include "velox/dwio/common/ScanSpec.h"
#include "velox/dwio/dwrf/reader/DwrfReader.h"
#include "velox/dwio/dwrf/writer/Writer.h"
#include "velox/dwio/parquet/RegisterParquetReader.h"
#include "velox/exec/Operator.h"
#include "velox/exec/OperatorUtils.h"
#include "velox/exec/tests/utils/Cursor.h"
#include "velox/expression/Expr.h"
#include "velox/functions/prestosql/aggregates/SumAggregate.h"
#include "velox/functions/prestosql/registration/RegistrationFunctions.h"
#include "velox/substrait/SubstraitToVeloxPlan.h"
#include "velox/substrait/TypeUtils.h"
#include "velox/type/Filter.h"
#include "velox/type/Subfield.h"

using namespace facebook::velox;
using namespace facebook::velox::exec;

namespace velox::execution {

std::shared_ptr<core::QueryCtx> createNewVeloxQueryCtx(
        memory::MemoryPool* memoryPool);

class VeloxInitializer {
public:
    VeloxInitializer() {
        Init();
    }
    void Init();
};

class PetrelVeloxColumnarBatch : public petrel::memory::PetrelColumnarBatch {
public:
    PetrelVeloxColumnarBatch(RowVectorPtr rowVector)
            : petrel::memory::PetrelColumnarBatch(
            rowVector->childrenSize(),
            rowVector->size()),
              rowVector_(rowVector) {}

    void ReleasePayload() override;
    std::string GetType() override;
    /// This method converts Velox RowVector into Arrow Array based on Velox's
    /// Arrow conversion implementation, in which memcopy is not needed for
    /// fixed-width data types, but is conducted in String conversion. The output
    /// array will be the input of Columnar Shuffle or Velox-to-Row.
    std::shared_ptr<ArrowArray> exportToArrow() override;

private:
    RowVectorPtr rowVector_;
};

class WholeStageResIter {
public:
    WholeStageResIter(
            std::shared_ptr<memory::MemoryPool> pool,
            std::shared_ptr<const core::PlanNode> planNode,
            const std::unordered_map<std::string, std::string>& confMap)
            : pool_(pool), planNode_(planNode), confMap_(confMap) {
        getOrderedNodeIds(planNode_, orderedNodeIds_);
    }

    virtual ~WholeStageResIter() {}

    arrow::Result<std::shared_ptr<PetrelVeloxColumnarBatch>> Next();

    std::shared_ptr<Metrics> GetMetrics(int64_t exportNanos) {
        collectMetrics();
        metrics_->veloxToArrow = exportNanos;
        return metrics_;
    }

    memory::MemoryPool* getPool() const;

    /// Set the Spark confs to Velox query context.
    void setConfToQueryContext(const std::shared_ptr<core::QueryCtx>& queryCtx);

    std::shared_ptr<exec::Task> task_;

    std::function<void(exec::Task*)> addSplits_;

    std::shared_ptr<const core::PlanNode> planNode_;

private:
    /// Get all the children plan node ids with postorder traversal.
    void getOrderedNodeIds(
            const std::shared_ptr<const core::PlanNode>& planNode,
            std::vector<core::PlanNodeId>& nodeIds);

    /// Collect Velox metrics.
    void collectMetrics();

    /// Return the sum of one runtime metric.
    int64_t sumOfRuntimeMetric(
            const std::unordered_map<std::string, RuntimeMetric>& runtimeStats,
            const std::string& metricId) const;

    std::shared_ptr<memory::MemoryPool> pool_;

    std::shared_ptr<Metrics> metrics_ = nullptr;
    int64_t metricVeloxToArrowNanos_ = 0;

    /// All the children plan node ids with postorder traversal.
    std::vector<core::PlanNodeId> orderedNodeIds_;

    /// Node ids should be ommited in metrics.
    std::unordered_set<core::PlanNodeId> omittedNodeIds_;

    /// A map of custome configs.
    std::unordered_map<std::string, std::string> confMap_;
};

// This class is used to convert the Substrait plan into Velox plan.
class VeloxPlanConverter : public petrel::execution::ExecBackendBase {
public:
    VeloxPlanConverter(
            const std::unordered_map<std::string, std::string>& confMap)
            : confMap_(confMap) {}

    std::shared_ptr<petrel::execution::PetrelResultIterator> GetResultIterator(
            petrel::memory::MemoryAllocator* allocator) override;

    std::shared_ptr<petrel::execution::PetrelResultIterator> GetResultIterator(
            petrel::memory::MemoryAllocator* allocator,
            std::vector<std::shared_ptr<petrel::execution::PetrelResultIterator>> inputs)
    override;

    // Used by unit test and benchmark.
    std::shared_ptr<petrel::execution::PetrelResultIterator> GetResultIterator(
            petrel::memory::MemoryAllocator* allocator,
            const std::vector<std::shared_ptr<facebook::velox::substrait::SubstraitVeloxPlanConverter::SplitInfo>>&
            scanInfos);

    // TODO: no c2r for now
    /*std::shared_ptr<petrel::columnartorow::ColumnarToRowConverterBase>
    getColumnarConverter(
            petrel::memory::MemoryAllocator* allocator,
            std::shared_ptr<arrow::RecordBatch> rb,
            bool wsChild) override {
        auto arrowPool = petrel::memory::AsWrappedArrowMemoryPool(allocator);
        auto veloxPool = petrel::memory::AsWrappedVeloxMemoryPool(allocator);
        if (wsChild) {
            return std::make_shared<VeloxToRowConverter>(rb, arrowPool, veloxPool);
        } else {
            // If the child is not Velox output, use Arrow-to-Row conversion instead.
            return std::make_shared<
                    petrel::columnartorow::ArrowColumnarToRowConverter>(rb, arrowPool);
        }
    }*/

    /// Separate the scan ids and stream ids, and get the scan infos.
    void getInfoAndIds(
            std::unordered_map<
                    core::PlanNodeId,
                    std::shared_ptr<facebook::velox::substrait::SubstraitVeloxPlanConverter::SplitInfo>> splitInfoMap,
            std::unordered_set<core::PlanNodeId> leafPlanNodeIds,
            std::vector<std::shared_ptr<facebook::velox::substrait::SubstraitVeloxPlanConverter::SplitInfo>>&
            scanInfos,
            std::vector<core::PlanNodeId>& scanIds,
            std::vector<core::PlanNodeId>& streamIds);

    std::shared_ptr<Metrics> GetMetrics(void* raw_iter, int64_t exportNanos)
    override {
        auto iter = static_cast<WholeStageResIter*>(raw_iter);
        return iter->GetMetrics(exportNanos);
    }

    std::shared_ptr<arrow::Schema> GetOutputSchema() override;

private:
    void setInputPlanNode(const ::substrait::AggregateRel& sagg);

    void setInputPlanNode(const ::substrait::ProjectRel& sproject);

    void setInputPlanNode(const ::substrait::FilterRel& sfilter);

    void setInputPlanNode(const ::substrait::JoinRel& sJoin);

    void setInputPlanNode(const ::substrait::ReadRel& sread);

    void setInputPlanNode(const ::substrait::Rel& srel);

    void setInputPlanNode(const ::substrait::RelRoot& sroot);

    std::shared_ptr<const core::PlanNode> getVeloxPlanNode(
            const ::substrait::Plan& splan);

    std::string nextPlanNodeId();

    void cacheOutputSchema(const std::shared_ptr<const core::PlanNode>& planNode);

    /* Result Iterator */
    class WholeStageResIterFirstStage;

    class WholeStageResIterMiddleStage;

    int planNodeId_ = 0;

    std::unordered_map<std::string, std::string> confMap_;

    std::vector<std::shared_ptr<petrel::execution::PetrelResultIterator>> arrowInputIters_;

    std::shared_ptr<facebook::velox::substrait::SubstraitParser> subParser_ =
            std::make_shared<facebook::velox::substrait::SubstraitParser>();

    std::shared_ptr<facebook::velox::substrait::SubstraitVeloxPlanConverter>
            subVeloxPlanConverter_ = std::make_shared<
            facebook::velox::substrait::SubstraitVeloxPlanConverter>(
            petrel::memory::GetDefaultWrappedVeloxMemoryPool().get());

    // Cache for tests/benchmark purpose.
    std::shared_ptr<const core::PlanNode> planNode_;
    std::shared_ptr<arrow::Schema> output_schema_;
};

} // namespace velox::execution
