#ifndef MACVELOX_COLUMNAR_BATCH_H
#define MACVELOX_COLUMNAR_BATCH_H

#include <memory>
#include <string>

#include "arrow/c/helpers.h"

namespace petrel::memory {

class PetrelColumnarBatch {
public:
    PetrelColumnarBatch(int32_t numColumns, int32_t numRows)
            : numColumns(numColumns), numRows(numRows) {}

    int32_t GetNumColumns() const {
        return numColumns;
    }

    int32_t GetNumRows() const {
        return numRows;
    }

    virtual void ReleasePayload() = 0;

    virtual std::string GetType() = 0;

    virtual std::shared_ptr<ArrowArray> exportToArrow() = 0;

    virtual int64_t getExportNanos() const {
        return exportNanos_;
    };

private:
    int32_t numColumns;
    int32_t numRows;

protected:
    int64_t exportNanos_;
};

class PetrelArrowArrayColumnarBatch : public PetrelColumnarBatch {
public:
    PetrelArrowArrayColumnarBatch(const ArrowArray& cArray)
            : PetrelColumnarBatch(cArray.n_children, cArray.length),
              cArray_(cArray) {}

    void ReleasePayload() override {
        ArrowArrayRelease(&cArray_);
    }

    std::string GetType() override {
        return "arrow_array";
    }

    std::shared_ptr<ArrowArray> exportToArrow() override {
        return std::make_shared<ArrowArray>(cArray_);
    }

private:
    ArrowArray cArray_;
};

} // petrel::memory

#endif //MACVELOX_COLUMNAR_BATCH_H
