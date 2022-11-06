set(VELOX_BACKEND_LIB_NAME "velox_backend")
set(VELOX_BACKEND_SRC
        memory/velox_memory_pool.cpp
        memory/allocator.cpp)
        
add_library(${VELOX_BACKEND_LIB_NAME} STATIC ${VELOX_BACKEND_SRC})
        
target_link_libraries(${VELOX_BACKEND_LIB_NAME}
        arrow
        velox_memory
        velox_substrait_plan_converter)
target_link_directories(${VELOX_BACKEND_LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/memory)
