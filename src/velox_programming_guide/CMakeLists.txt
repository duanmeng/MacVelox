add_executable(buffers_flat_vectors BuffersAndFlatVectors.cpp)
target_link_libraries(buffers_flat_vectors velox_buffer velox_vector)

add_executable(velox_demo VeloxDemo.cpp)
target_link_libraries(
  velox_demo
  velox_aggregates
  velox_type
  velox_vector
  velox_vector_test_lib
  velox_exec
  velox_exec_test_lib
  velox_tpch_connector
  velox_memory
  folly)
