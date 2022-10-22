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

