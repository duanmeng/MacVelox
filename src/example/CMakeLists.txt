find_library(RE2 re2 REQUIRED)

add_executable(mac_hello_velox HelloVelox.cpp)
target_link_libraries(mac_hello_velox velox_type velox_vector velox_expression
                      velox_memory)

add_executable(mac_hello_arrow HelloArrow.cpp)
target_link_libraries(mac_hello_arrow arrow parquet)

add_executable(mac_hello_xsimd HelloXsimd.cpp)
target_link_libraries(mac_hello_xsimd  xsimd)
