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

add_executable(velox_expression_eval_demo ExpressionEvalDemo.cpp)
target_link_libraries(velox_expression_eval_demo velox_type velox_vector
                      velox_memory velox_expression)

add_executable(velox_scan_sort_demo ScanAndSortDemo.cpp)
target_link_libraries(
  velox_scan_sort_demo
  velox_type
  velox_vector
  velox_exec
  velox_exec_test_lib
  velox_hive_connector
  velox_memory
  velox_vector_test_lib)

add_executable(velox_operator_extensibility_demo OperatorExtensibilityDemo.cpp)
target_link_libraries(
  velox_operator_extensibility_demo
  velox_type
  velox_vector
  velox_exec
  velox_exec_test_lib
  velox_memory
  velox_vector_test_lib)

add_executable(velox_scan_orc_demo ScanOrcDemo.cpp)
target_link_libraries(
  velox_scan_orc_demo
  velox_type
  velox_vector
  velox_exec
  velox_exec_test_lib
  velox_hive_connector
  velox_memory)

add_executable(velox_vector_reader_writer_demo VectorReaderWriterDemo.cpp)
target_link_libraries(velox_vector_reader_writer_demo velox_expression
                      velox_type velox_vector)
