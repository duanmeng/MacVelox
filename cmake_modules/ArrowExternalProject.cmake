option(STATIC_ARROW "Build Arrow with Static Libraries" ON)

set(ARROW_LIB_NAME "arrow")
set(PARQUET_LIB_NAME "parquet")
set(ARROW_DATASET_LIB_NAME "arrow_dataset")
set(ARROW_SUBSTRAIT_LIB_NAME "arrow_substrait")


set(ARROW_EP_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/arrow_ep/install")
set(ARROW_EP_INSTALL_LIB ${ARROW_EP_INSTALL_PREFIX}/lib)
set(ARROW_EP_INSTALL_INCLUDE ${ARROW_EP_INSTALL_PREFIX}/include)

set(ARROW_LIB
        ${ARROW_EP_INSTALL_LIB}/${CMAKE_STATIC_LIBRARY_PREFIX}${ARROW_LIB_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
set(PARQUET_LIB
        ${ARROW_EP_INSTALL_LIB}/${CMAKE_STATIC_LIBRARY_PREFIX}${PARQUET_LIB_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
set(ARROW_DATASET_LIB
        ${ARROW_EP_INSTALL_LIB}/${CMAKE_STATIC_LIBRARY_PREFIX}${ARROW_DATASET_LIB_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
set(ARROW_SUBSTRAIT_LIB
        ${ARROW_EP_INSTALL_LIB}/${CMAKE_STATIC_LIBRARY_PREFIX}${ARROW_SUBSTRAIT_LIB_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})

function(FIND_ARROW_LIB)
    set(ARROW_LIB_FULL_NAME
            ${CMAKE_STATIC_LIBRARY_PREFIX}${ARROW_LIB_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(PARQUET_LIB_FULL_NAME
            ${CMAKE_STATIC_LIBRARY_PREFIX}${PARQUET_LIB_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(ARROW_DATASET_LIB_FULL_NAME
            ${CMAKE_STATIC_LIBRARY_PREFIX}${ARROW_DATASET_LIB_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(ARROW_SUBSTRAIT_LIB_FULL_NAME
            ${CMAKE_STATIC_LIBRARY_PREFIX}${ARROW_SUBSTRAIT_LIB_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})

    set(ARROW_IMPORTED_LOCATION  "${CMAKE_BINARY_DIR}/lib")
    set(ARROW_INCLUDE_DIRECTORIES  "${CMAKE_BINARY_DIR}/include")

    add_library(${ARROW_LIB_NAME} STATIC IMPORTED)
    add_library(${PARQUET_LIB_NAME} STATIC IMPORTED GLOBAL)
    add_dependencies(${ARROW_LIB_NAME} arrow_external_project)
    add_dependencies(${PARQUET_LIB_NAME} ${ARROW_LIB_NAME})

    set_target_properties(${ARROW_LIB_NAME}
            PROPERTIES IMPORTED_LOCATION "${ARROW_IMPORTED_LOCATION}/${ARROW_LIB_FULL_NAME}"
            INTERFACE_INCLUDE_DIRECTORIES "${ARROW_INCLUDE_DIRECTORIES}")
    set_target_properties(${PARQUET_LIB_NAME}
            PROPERTIES IMPORTED_LOCATION "${ARROW_IMPORTED_LOCATION}/${PARQUET_LIB_FULL_NAME}"
            INTERFACE_INCLUDE_DIRECTORIES "${ARROW_INCLUDE_DIRECTORIES}")

    if (NOT EXISTS ${CMAKE_BINARY_DIR}/include)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
    endif()
    add_custom_command(
            OUTPUT
            ${CMAKE_BINARY_DIR}/lib/${ARROW_LIB_FULL_NAME}
            ${CMAKE_BINARY_DIR}/lib/${PARQUET_LIB_FULL_NAME}
            ${CMAKE_BINARY_DIR}/lib/${ARROW_DATASET_LIB_FULL_NAME}
            ${CMAKE_BINARY_DIR}/lib/${ARROW_SUBSTRAIT_LIB_FULL_NAME}
            COMMAND cp -a ${ARROW_LIB} ${ARROW_IMPORTED_LOCATION}
            COMMAND cp -a ${PARQUET_LIB} ${ARROW_IMPORTED_LOCATION}
            COMMAND cp -a ${ARROW_DATASET_LIB} ${ARROW_IMPORTED_LOCATION}
            COMMAND cp -a ${ARROW_SUBSTRAIT_LIB} ${ARROW_IMPORTED_LOCATION}
            COMMAND cp -r ${ARROW_EP_INSTALL_INCLUDE} ${CMAKE_BINARY_DIR}
            DEPENDS arrow_external_project)
    add_custom_target(
            COPY_LIB_HEADERS
            ALL
            DEPENDS
            ${ARROW_IMPORTED_LOCATION}/${ARROW_LIB_FULL_NAME}
            ${ARROW_IMPORTED_LOCATION}/${PARQUET_LIB_FULL_NAME}
            ${ARROW_IMPORTED_LOCATION}/${ARROW_DATASET_LIB_FULL_NAME}
            ${ARROW_IMPORTED_LOCATION}/${ARROW_SUBSTRAIT_LIB_FULL_NAME})
    add_dependencies(${ARROW_LIB_NAME} COPY_LIB_HEADERS)
    add_dependencies(${PARQUET_LIB_NAME} COPY_LIB_HEADERS)
endfunction()

function(ADD_ARROW_EP)
    find_package(Thrift)
    if(Thrift_FOUND)
        set(THRIFT_SOURCE "SYSTEM")
    else()
        set(THRIFT_SOURCE "BUNDLED")
    endif()

    set(ARROW_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/arrow_ep")
    set(ARROW_LIBDIR ${ARROW_PREFIX}/install/${CMAKE_INSTALL_LIBDIR}) # CMAKE_INSTALL_LIBDIR = lib

    add_library(thrift STATIC IMPORTED GLOBAL)
    if(NOT Thrift_FOUND)
        set(THRIFT_ROOT ${ARROW_PREFIX}/src/arrow_ep-build/thrift_ep-install)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(THRIFT_LIB ${THRIFT_ROOT}/lib/libthriftd.a)
        else()
            set(THRIFT_LIB ${THRIFT_ROOT}/lib/libthrift.a)
        endif()

        file(MAKE_DIRECTORY ${THRIFT_ROOT}/include)
        set(THRIFT_INCLUDE_DIR ${THRIFT_ROOT}/include)
    endif()

    set_property(TARGET thrift PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${THRIFT_INCLUDE_DIR})
    set_property(TARGET thrift PROPERTY IMPORTED_LOCATION ${THRIFT_LIB})
    message(STATUS "Thrift use ${THRIFT_SOURCE} include dir is ${THRIFT_INCLUDE_DIR} lib path is ${THRIFT_LIB}")

    set(ARROW_CMAKE_ARGS
            -DARROW_PARQUET=ON
            -DARROW_WITH_LZ4=ON
            -DARROW_WITH_SNAPPY=ON
            -DARROW_WITH_ZLIB=ON
            -DARROW_WITH_ZSTD=ON
            -DARROW_JEMALLOC=OFF
            -DARROW_SIMD_LEVEL=NONE
            -DARROW_RUNTIME_SIMD_LEVEL=NONE
            -DARROW_WITH_UTF8PROC=OFF
            -DCMAKE_INSTALL_PREFIX=${ARROW_PREFIX}/install
            -DARROW_BUILD_STATIC=ON
            -DThrift_SOURCE=${THRIFT_SOURCE}
            -DARROW_SUBSTRAIT=ON
            -DARROW_COMPUTE=ON
            -DARROW_CSV=ON
            -DARROW_DATASET=ON
            -DARROW_JSON=ON)
    ExternalProject_Add(
            arrow_external_project
            PREFIX ${ARROW_PREFIX}
            URL "https://dlcdn.apache.org/arrow/arrow-8.0.0/apache-arrow-8.0.0.tar.gz"
            SOURCE_SUBDIR cpp
            CMAKE_ARGS ${ARROW_CMAKE_ARGS}
            BUILD_BYPRODUCTS ${ARROW_LIBDIR}/libarrow.a ${ARROW_LIBDIR}/libparquet.a
            ${THRIFT_LIB})
endfunction()

include(ExternalProject)

add_arrow_ep()
find_arrow_lib()
message(STATUS "find_arrow_lib finished")
