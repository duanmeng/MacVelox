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

SHELL := /bin/bash

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

clean: cleansub	#: Delete all build artifacts
	rm -rf $(BUILD_BASE_DIR)

velox-submodule:		#: Check out code for velox submodule
	git submodule sync --recursive
	git submodule update --init --recursive

submodules: velox-submodule

cleansub:
	sed -i '' 's/PROJECT_SOURCE_DIR/CMAKE_SOURCE_DIR/g' $(MAKEFILE_DIR)/velox/velox/substrait/CMakeLists.txt

cmake:	#: Use CMake to create a Makefile build system
	# TODO: Velox's substrait use CMAKE_SOURCE_DIR as the proto_path ($VeloxProjectDir/velox),
	# it works only when it is the top-level project not when it is used as a sub-project,
	# in which the proto_path would have one more layer ($OtherProjectDir/velox/velox).
	# This is just a workaround, and we should fix it in the Velox project itself.
	sed -i '' 's/CMAKE_SOURCE_DIR/PROJECT_SOURCE_DIR/g' $(MAKEFILE_DIR)/velox/velox/substrait/CMakeLists.txt
	cmake -B "$(BUILD_BASE_DIR)/$(BUILD_DIR)" $(FORCE_COLOR) $(CMAKE_FLAGS)

build:	#: Build the software based in BUILD_DIR and BUILD_TYPE variables
	cmake --build $(BUILD_BASE_DIR)/$(BUILD_DIR) -j $(NUM_THREADS)

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
