#pragma once

#include <arrow/type_fwd.h>

#include "velox/type/Type.h"

using namespace facebook::velox;

std::shared_ptr<arrow::DataType> toArrowTypeFromName(
        const std::string& type_name);

std::shared_ptr<arrow::DataType> toArrowType(const TypePtr& type);

const char* arrowTypeIdToFormatStr(arrow::Type::type typeId);

std::shared_ptr<arrow::Schema> toArrowSchema(
        const std::shared_ptr<const RowType>& row_type);
