set(VELOX_BACKEND_LIB_NAME "velox_backend")
set(VELOX_BACKEND_SRC
        memory/velox_memory_pool.cpp
        memory/allocator.cpp
        memory/arrow_memory_pool.cpp
        execution/protobuf_utils.cpp
        execution/exec_backend.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(utils)

message(STATUS "MACDUAN Protobuf_USE_STATIC_LIBS - ${Protobuf_USE_STATIC_LIBS}")
message(STATUS "MACDUAN Protobuf_LIBRARIES - ${Protobuf_LIBRARIES}")
message(STATUS "MACDUAN Protobuf_LIBRARIES - ${Protobuf_LIBRARIES}")

include_directories(${Protobuf_INCLUDE_DIRS})

add_library(${VELOX_BACKEND_LIB_NAME} STATIC ${VELOX_BACKEND_SRC})
        
target_link_libraries(${VELOX_BACKEND_LIB_NAME}
        arrow
        velox_memory
        velox_substrait_plan_converter
        protobuf)
target_link_directories(${VELOX_BACKEND_LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/memory)
