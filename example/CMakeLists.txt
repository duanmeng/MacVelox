find_library(RE2 re2 REQUIRED)

add_executable(mac_velox_example_scan_and_sort MacScanAndSort.cpp)
target_link_libraries(
  mac_velox_example_scan_and_sort
  velox_type
  velox_vector
  velox_exec
  velox_exec_test_lib
  velox_hive_connector
  velox_memory)

add_executable(mac_hello_velox HelloVelox.cpp)
target_link_libraries(mac_hello_velox
        velox_memory
        velox_functions_lib
        velox_core
        velox_expression ${RE2})

add_executable(mac_hello_arrow HelloArrow.cpp)
target_link_libraries(mac_hello_arrow arrow parquet)
