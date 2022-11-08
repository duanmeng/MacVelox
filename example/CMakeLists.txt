find_library(RE2 re2 REQUIRED)

add_executable(mac_hello_velox HelloVelox.cpp)
target_link_libraries(mac_hello_velox
        velox_memory
        velox_functions_lib
        velox_core
        velox_expression ${RE2})

add_executable(mac_hello_arrow HelloArrow.cpp)
target_link_libraries(mac_hello_arrow arrow parquet)
