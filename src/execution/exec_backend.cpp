#include "exec_backend.h"

#include <mutex>
#include <utility>

namespace petrel::execution {

static std::function<std::shared_ptr<ExecBackendBase>()> backend_factory;

void SetBackendFactory(
        std::function<std::shared_ptr<ExecBackendBase>()> factory) {
#ifdef PETREL_PRINT_DEBUG
    std::cout << "Set backend factory." << std::endl;
#endif
    backend_factory = std::move(factory);
}

std::shared_ptr<ExecBackendBase> CreateBackend() {
    if (backend_factory == nullptr) {
        throw std::runtime_error(
                "Execution backend not set. This may due to the backend library not loaded, or SetBackendFactory() is not called in nativeInitNative() JNI call.");
    }
    return backend_factory();
}

} // namespace gluten