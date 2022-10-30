#!/bin/bash
set -eufx -o pipefail

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
