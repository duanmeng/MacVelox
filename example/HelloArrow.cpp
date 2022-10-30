#include "arrow/status.h"
#include "parquet/bloom_filter.h"
#include "parquet/hasher.h"
#include "parquet/murmur3.h"

#include <iostream>

int main(int argc, char** argv) {
    arrow::StatusCode sc = arrow::StatusCode::SerializationError;
    std::cout << "Hello Arrow arrow::StatusCode::SerializationError string is "
        << arrow::Status::CodeAsString(sc) << std::endl;

    int64_t magic = 0xdeadbeaf;
    auto blockSplitBloomFilter = std::make_unique<parquet::BlockSplitBloomFilter>();
    blockSplitBloomFilter->Init(1024);
    std::unique_ptr<parquet::Hasher> murmurHash3 = std::make_unique<parquet::MurmurHash3>();
    murmurHash3->Hash(123);
    std::cout << "Hello arrow/parquet magic number "
            << magic
            << " BlockSplitBloomFilter hash value is "
            << blockSplitBloomFilter->Hash(magic)
            << " MurmurHash3 value is "
            << murmurHash3->Hash(magic)
            << std::endl;
    return 0;
}
