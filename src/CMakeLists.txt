set(VELOX_BACKEND_LIB_NAME "velox_backend")
set(VELOX_BACKEND_SRC
        execution/ArrowTypeUtils.cpp
        execution/bridge.cpp
        execution/DwrfDatasource.cpp
        execution/exec_backend.cpp
        execution/protobuf_utils.cpp
        execution/VeloxPlanConverter.cpp
        memory/allocator.cpp
        memory/arrow_memory_pool.cpp
        memory/velox_memory_pool.cpp)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/execution
        ${CMAKE_CURRENT_SOURCE_DIR}/memory
        ${CMAKE_CURRENT_SOURCE_DIR}/utils)

include_directories(${Protobuf_INCLUDE_DIRS})

add_library(${VELOX_BACKEND_LIB_NAME} STATIC ${VELOX_BACKEND_SRC})
        
target_link_libraries(${VELOX_BACKEND_LIB_NAME}
        arrow
        velox_memory
        velox_substrait_plan_converter
        velox_functions_prestosql
        velox_functions_spark
        velox_aggregates
        protobuf)
target_link_directories(${VELOX_BACKEND_LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/memory)
