#include "velox/common/memory/Memory.h"
#include "velox/exec/Task.h"
#include "velox/exec/tests/utils/PlanBuilder.h"
#include "velox/exec/tests/utils/TempDirectoryPath.h"
#include "velox/type/Type.h"
#include "velox/vector/BaseVector.h"

#include <folly/init/Init.h>
#include <algorithm>
#include <iostream>

using namespace facebook::velox;

int main(int argc, char** argv) {
  // Velox Tasks/Operators are based on folly's async framework, so we need to
  // make sure we initialize it first.
  folly::init(&argc, &argv);

  // Default memory allocator used throughout this example.
  auto pool = memory::getDefaultScopedMemoryPool();
  std::cout << "Hello MacVelox!\n";

  return 0;
}

