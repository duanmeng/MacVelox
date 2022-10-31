#!/bin/bash
set -eufx -o pipefail

function setup_cpu_target_arch {
  local OS
  OS=$(uname)
  local MACHINE
  MACHINE=$(uname -m)
  CPU_ARCH=""
  CPU_TARGET=$CPU_ARCH

  echo "defult CPU_ARCH=$CPU_ARCH"
  echo "defult CPU_TARGET=$CPU_TARGET"

  if [ -z "$CPU_ARCH" ]; then
    if [ "$OS" = "Darwin" ]; then
      if [ "$MACHINE" = "x86_64" ]; then
        local CPU_CAPABILITIES
        CPU_CAPABILITIES=$(sysctl -a | grep machdep.cpu.features | awk '{print tolower($0)}')

        if [[ $CPU_CAPABILITIES =~ "avx" ]]; then
          CPU_ARCH="avx"
        else
          CPU_ARCH="sse"
        fi

      elif [[ $(sysctl -a | grep machdep.cpu.brand_string) =~ "Apple" ]]; then
        # Apple silicon.
        CPU_ARCH="arm64"
      fi

    else [ "$OS" = "Linux" ];
      local CPU_CAPABILITIES
      CPU_CAPABILITIES=$(cat /proc/cpuinfo | grep flags | head -n 1| awk '{print tolower($0)}')

      if [[ "$CPU_CAPABILITIES" =~ "avx" ]]; then
            CPU_ARCH="avx"
      elif [[ "$CPU_CAPABILITIES" =~ "sse" ]]; then
            CPU_ARCH="sse"
      elif [ "$MACHINE" = "aarch64" ]; then
            CPU_ARCH="aarch64"
      fi
    fi
  fi
  CPU_TARGET=$CPU_ARCH

  echo "set CPU_ARCH=$CPU_ARCH"
  echo "set CPU_TARGET=$CPU_TARGET"
}

setup_cpu_target_arch

git submodule sync --recursive
git submodule update --init --recursive

BASE_DIR=$(pwd)
# Run the velox setup script first.
source "${BASE_DIR}/velox/scripts/setup-macos.sh"

MACOS_DEPS="${MACOS_DEPS} bison gperf libsodium"

function install_six {
  pip3 install six
}

export PATH=$(brew --prefix bison)/bin:$PATH


function install_macduan_deps {
  install_velox_deps
}

install_macduan_deps

echo "Finish Env Setup"
