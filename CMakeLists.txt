cmake_minimum_required(VERSION 3.10)

# set the project name
project(mac_velox)

set(VELOX_ROOT ${CMAKE_BINARY_DIR}/velox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -mavx2 -mfma -mavx -mf16c -masm=intel -mlzcnt")
if("${TREAT_WARNINGS_AS_ERRORS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()


set(VELOX_ENABLE_PARQUET
  ON
  CACHE BOOL "Enable Parquet support")
add_definitions(-DPRESTO_ENABLE_PARQUET)

set(VELOX_BUILD_TESTING
    ON
    CACHE BOOL "Enable Velox tests")

set(VELOX_ENABLE_SPARK_FUNCTIONS
    OFF
    CACHE BOOL "Enable Velox Spark functions")

set(VELOX_ENABLE_EXAMPLES
    OFF
    CACHE BOOL "Enable Velox examples")

set(VELOX_BUILD_TEST_UTILS
    OFF
    CACHE BOOL "Enable Velox test utils")

include_directories(.)
include_directories(velox)
include_directories(${VELOX_ROOT})

add_subdirectory(velox)

if(ENABLE_TESTING)
  include(CTest) # include after project() but before add_subdirectory()
  include_directories(velox/third_party/googletest/googletest/include)
  include_directories(veloxs/third_party/googletest/googlemock/include)
endif()

add_subdirectory(example)
