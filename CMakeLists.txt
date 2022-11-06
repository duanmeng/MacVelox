cmake_minimum_required(VERSION 3.10)

# set the project name
project(MacVelox)

set(VELOX_ROOT ${CMAKE_BINARY_DIR}/velox)

execute_process(
  COMMAND
    bash -c
    "( source ${CMAKE_CURRENT_SOURCE_DIR}/velox/scripts/setup-helper-functions.sh && echo -n $(get_cxx_flags $ENV{CPU_TARGET}))"
  OUTPUT_VARIABLE SCRIPT_CXX_FLAGS
  RESULT_VARIABLE COMMAND_STATUS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
message("Appending CMAKE_CXX_FLAGS with ${SCRIPT_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SCRIPT_CXX_FLAGS}")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

if("${TREAT_WARNINGS_AS_ERRORS}")
  set(KNOWN_WARNINGS "-Wno-nullability-completeness")

  # known warnings for MacOS
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(KNOWN_WARNINGS "${KNOWN_WARNINGS} -Wno-deprecated-declarations")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror ${KNOWN_WARNINGS}")
endif()

if(VELOX_ENABLE_PARQUET)
  message(INFO " velox parquet enabled")
  set(VELOX_ENABLE_PARQUET
    ON
    CACHE BOOL "Enable Parquet support")
  add_definitions(-DVELOX_ENABLE_PARQUET)
endif()

if(VELOX_ENABLE_HDFS)
  message(INFO " velox hdfs enabled")
  set(VELOX_ENABLE_HDFS
    OFF
    CACHE BOOL "Build HDFS Connector")
  add_definitions(-DVELOX_ENABLE_HDFS)
endif()

set(VELOX_BUILD_TESTING
    OFF
    CACHE BOOL "Enable Velox tests")

set(VELOX_ENABLE_SPARK_FUNCTIONS
    ON
    CACHE BOOL "Enable Velox Spark functions")

set(VELOX_ENABLE_EXAMPLES
    OFF
    CACHE BOOL "Enable Velox examples")

set(VELOX_BUILD_TEST_UTILS
    ON
    CACHE BOOL "Enable Velox test utils")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VELOX_ENABLE_SUBSTRAIT ON CACHE BOOL "Enable Velox Substrait")

set(Boost_USE_MULTITHREADED TRUE)
find_package(
  Boost
  1.66.0
  REQUIRED
  program_options
  context
  filesystem
  regex
  thread
  system
  date_time
  atomic)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

find_package(gflags COMPONENTS shared)

find_library(GLOG glog)
find_library(FMT fmt)
find_library(EVENT event)
find_library(DOUBLE_CONVERSION double-conversion)
find_library(LZ4 lz4)
find_library(LZO lzo2)
find_library(ZSTD zstd)
find_package(ZLIB)
find_library(SNAPPY snappy)

find_package(folly CONFIG REQUIRED)
set(FOLLY_WITH_DEPENDENCIES
    ${FOLLY_LIBRARIES}
    ${DOUBLE_CONVERSION}
    Boost::context
    dl
    ${EVENT}
    ${SNAPPY}
    ${LZ4}
    ${ZSTD}
    ${ZLIB_LIBRARIES})

find_package(BZip2 MODULE)
if(BZIP2_FOUND)
  list(APPEND FOLLY_WITH_DEPENDENCIES ${BZIP2_LIBRARIES})
endif()
include_directories(SYSTEM ${FOLLY_INCLUDE_DIRS})

# Include third party header files

find_package(Protobuf REQUIRED)

find_path(OPT_OPENSSL_DIR NAMES opt/openssl)
set(OPENSSL_ROOT_DIR "${OPT_OPENSSL_DIR}/opt/openssl")
find_package(OpenSSL REQUIRED)

include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})
include_directories(.)
include_directories(velox)
include_directories(velox/velox/external/xxhash)
include_directories(${VELOX_ROOT})

add_subdirectory(velox)

if(ENABLE_TESTING)
  include(CTest) # include after project() but before add_subdirectory()
  include_directories(velox/third_party/googletest/googletest/include)
  include_directories(veloxs/third_party/googletest/googlemock/include)
endif()

#
# Install arrow related libraries by using external project
#
include(ArrowExternalProject)

add_subdirectory(example)
add_subdirectory(src)

