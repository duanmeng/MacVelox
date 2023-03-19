.PHONY: all cmake build clean debug release submodules velox-submodule

BUILD_BASE_DIR=_build
BUILD_DIR=release
BUILD_TYPE=Release
TREAT_WARNINGS_AS_ERRORS ?= 1
ENABLE_WALL ?= 1
NUM_THREADS ?= $(shell getconf _NPROCESSORS_CONF 2>/dev/null || echo 1)
CPU_TARGET ?= "avx" # just a hint, velox will get the actual cpu arch on the fly
CMAKE_PREFIX_PATH ?= "/usr/local"

# turn off it use the arrow and parquet in external project
VELOX_ENABLE_PARQUET ?= "OFF"
VELOX_ENABLE_HDFS ?= "OFF" # TODO: enable it

CMAKE_FLAGS := -DTREAT_WARNINGS_AS_ERRORS=${TREAT_WARNINGS_AS_ERRORS}
CMAKE_FLAGS += -DENABLE_ALL_WARNINGS=${ENABLE_WALL}
CMAKE_FLAGS += -DCMAKE_PREFIX_PATH=$(CMAKE_PREFIX_PATH)
CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
CMAKE_FLAGS += -DVELOX_ENABLE_PARQUET=$(VELOX_ENABLE_PARQUET)
CMAKE_FLAGS += -DVELOX_ENABLE_HDFS=$(VELOX_ENABLE_HDFS)

MAKEFILE_DIR := $(shell pwd)

# Use Ninja if available. If Ninja is used, pass through parallelism control flags.
USE_NINJA ?= 1
ifeq ($(USE_NINJA), 1)
ifneq ($(shell which ninja), )
CMAKE_FLAGS += -GNinja -DMAX_LINK_JOBS=$(MAX_LINK_JOBS) -DMAX_HIGH_MEM_JOBS=$(MAX_HIGH_MEM_JOBS)
endif
endif

ifndef USE_CCACHE
ifneq ($(shell which ccache), )
CMAKE_FLAGS += -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
endif
endif

all: release			#: Build the release version

clean: #: Delete all build artifacts
	rm -rf $(BUILD_BASE_DIR)

submodules:
	git submodule sync --recursive
	git submodule update --init --recursive

update-submodules: submodules  #: Check out code for velox submodule
	git submodule update --remote --merge

cmake:	#: Use CMake to create a Makefile build system
	cmake -B "$(BUILD_BASE_DIR)/$(BUILD_DIR)" $(FORCE_COLOR) $(CMAKE_FLAGS)

build:	#: Build the software based in BUILD_DIR and BUILD_TYPE variables
	cmake --build $(BUILD_BASE_DIR)/$(BUILD_DIR) -j $(NUM_THREADS)

format-fix: 			#: Fix formatting issues in the main branch
	scripts/check.py format main --fix

format-check: 			#: Check for formatting issues on the main branch
	clang-format --version
	scripts/check.py format main

debug:					#: Build with debugging symbols
	$(MAKE) cmake BUILD_DIR=debug BUILD_TYPE=Debug
	$(MAKE) build BUILD_DIR=debug

release:				#: Build the release version
	$(MAKE) cmake BUILD_DIR=release BUILD_TYPE=Release && \
	$(MAKE) build BUILD_DIR=release

test:
	echo "Do Nothing"

help:					#: Show the help messages
	@cat $(firstword $(MAKEFILE_LIST)) | \
	awk '/^[-a-z]+:/' | \
	awk -F: '{ printf("%-20s   %s\n", $$1, $$NF) }'
