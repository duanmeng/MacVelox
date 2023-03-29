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
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(ResolveDependency)

set_with_default(VELOX_DEPENDENCY_SOURCE_DEFAULT VELOX_DEPENDENCY_SOURCE AUTO)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Add all options below
option(
  VELOX_BUILD_TESTING
  "Enable Velox tests. This will enable all other build options automatically."
  ON)
option(
  VELOX_BUILD_MINIMAL
  "Build a minimal set of components only. This will override other build options."
  OFF)
# option() always creates a BOOL variable so we have to use a normal cache
# variable with STRING type for this option.
#
# * AUTO: Try SYSTEM first fall back to BUNDLED.
# * SYSTEM: Use installed dependencies via find_package.
# * BUNDLED: Build dependencies from source.
set(VELOX_DEPENDENCY_SOURCE
    ${VELOX_DEPENDENCY_SOURCE_DEFAULT}
    CACHE
      STRING
      "Default source for all dependencies with source builds enabled: AUTO SYSTEM BUNDLED."
)

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

# MacOSX enables two-level namespace by default:
# http://mirror.informatimago.com/next/developer.apple.com/releasenotes/DeveloperTools/TwoLevelNamespaces.html
# Enables -flat_namespace so type_info can be deudplicated across .so boundaries
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_link_options("-Wl,-flat_namespace")
endif()

if(UNIX AND NOT APPLE)
  # codegen linker flags, -export-dynamic for rtti
  add_link_options("-Wl,-export-dynamic")
endif()

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm64" AND ${CMAKE_SYSTEM_NAME} MATCHES
                                                 "Darwin")
  set(ON_APPLE_M1 True)
endif()

# Required so velox code can be used in a dynamic library
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

execute_process(
  COMMAND
    bash -c
    "( source ${CMAKE_CURRENT_SOURCE_DIR}/scripts/setup-helper-functions.sh && echo -n $(get_cxx_flags $ENV{CPU_TARGET}))"
  OUTPUT_VARIABLE SCRIPT_CXX_FLAGS
  RESULT_VARIABLE COMMAND_STATUS)

if(COMMAND_STATUS EQUAL "1")
  message(FATAL_ERROR "Unable to determine compiler flags!")
endif()
message("Setting CMAKE_CXX_FLAGS=${SCRIPT_CXX_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SCRIPT_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D USE_VELOX_COMMON_BASE")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D HAS_UNCAUGHT_EXCEPTIONS")
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsigned-char")
endif()

# Under Ninja, we are able to designate certain targets large enough to require
# restricted parallelism.
if("${MAX_HIGH_MEM_JOBS}")
  set_property(GLOBAL PROPERTY JOB_POOLS
                               "high_memory_pool=${MAX_HIGH_MEM_JOBS}")
else()
  set_property(GLOBAL PROPERTY JOB_POOLS high_memory_pool=1000)
endif()

if("${MAX_LINK_JOBS}")
  set_property(GLOBAL APPEND PROPERTY JOB_POOLS
                                      "link_job_pool=${MAX_LINK_JOBS}")
  set(CMAKE_JOB_POOL_LINK link_job_pool)
endif()

if("${TREAT_WARNINGS_AS_ERRORS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

if("${ENABLE_ALL_WARNINGS}")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(KNOWN_COMPILER_SPECIFIC_WARNINGS
        "-Wno-range-loop-analysis \
         -Wno-mismatched-tags \
         -Wno-nullability-completeness")
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(KNOWN_COMPILER_SPECIFIC_WARNINGS
        "-Wno-implicit-fallthrough \
         -Wno-empty-body \
         -Wno-class-memaccess \
         -Wno-comment \
         -Wno-int-in-bool-context \
         -Wno-redundant-move \
         -Wno-array-bounds \
         -Wno-maybe-uninitialized \
         -Wno-unused-result \
         -Wno-format-overflow \
         -Wno-strict-aliasing \
         -Wno-type-limits \
         -Wno-stringop-overflow")
  endif()

  set(KNOWN_WARNINGS
      "-Wno-unused \
       -Wno-unused-parameter \
       -Wno-sign-compare \
       -Wno-ignored-qualifiers \
       ${KNOWN_COMPILER_SPECIFIC_WARNINGS}")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra ${KNOWN_WARNINGS}")
endif()

message("FINAL CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VELOX_ENABLE_SUBSTRAIT
    ON
    CACHE BOOL "Enable Velox Substrait")

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

set_source(gflags)
resolve_dependency(gflags COMPONENTS shared)
if(NOT TARGET gflags::gflags)
  # This is a bit convoluted, but we want to be able to use gflags::gflags as a
  # target even when velox is built as a subproject which uses
  # `find_package(gflags)` which does not create a globally imported target that
  # we can ALIAS.
  add_library(gflags_gflags INTERFACE)
  target_link_libraries(gflags_gflags INTERFACE gflags)
  add_library(gflags::gflags ALIAS gflags_gflags)
endif()

if(${gflags_SOURCE} STREQUAL "BUNDLED")
  # we force glog from source to avoid issues with a system version built
  # against another gflags version (which is likely)
  set(glog_SOURCE BUNDLED)
else()
  set(glog_SOURCE SYSTEM)
endif()
resolve_dependency(glog)

set_source(fmt)
resolve_dependency(fmt)

# Locate or build folly.
set_source(folly)
resolve_dependency(folly)

# If we use BUNDLED boost we need to set this manually AFTER folly is configured
if(NOT DEFINED Boost_LIBRARIES)
  set(Boost_LIBRARIES ${BOOST_INCLUDE_LIBRARIES})
  list(TRANSFORM Boost_LIBRARIES PREPEND Boost::)
endif()

set(FOLLY_WITH_DEPENDENCIES
    ${FOLLY_LIBRARIES}
    ${Boost_LIBRARIES}
    ${DOUBLE_CONVERSION}
    ${EVENT}
    ${SNAPPY}
    ${CMAKE_DL_LIBS}
    ${FMT})

if(DEFINED FOLLY_BENCHMARK_STATIC_LIB)
  set(FOLLY_BENCHMARK ${FOLLY_BENCHMARK_STATIC_LIB})
else()
  set(FOLLY_BENCHMARK Folly::follybenchmark)
endif()

if(NOT ${VELOX_BUILD_MINIMAL})
  find_package(BZip2 MODULE)
  if(BZIP2_FOUND)
    list(APPEND FOLLY_WITH_DEPENDENCIES ${BZIP2_LIBRARIES})
  endif()
endif()

include_directories(SYSTEM ${FOLLY_INCLUDE_DIRS})

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
include_directories(${CMAKE_BINARY_DIR}) # for dwio/dwrf

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

set(xsimd_SOURCE BUNDLED)
resolve_dependency(xsimd)

add_subdirectory(src)
