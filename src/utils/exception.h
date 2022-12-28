#pragma once

#include <stdexcept>
#include <string>

#include "arrow/status.h"

namespace petrel::utils {

class PetrelException : public std::runtime_error {
public:
    explicit PetrelException(const std::string& msg) : std::runtime_error(msg) {

    }

};

inline void throw_exception_if_not_ok(arrow::Status status) {
    arrow::Status s = arrow::internal::GenericToStatus(status);
    if (!s.ok()) {
        throw PetrelException(s.ToString());
    }
}

} // namespace petrel::utils
