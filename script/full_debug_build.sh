#!/bin/bash

if [ -z "${CC}" -o -z "$CXX" ]; then
    echo "ERROR: Compilers are not provided"
    exit 1
fi

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR=$( dirname ${SCRIPT_DIR} )

if [ -z ${BUILD_DIR} ]; then
    export BUILD_DIR="${ROOT_DIR}/build.full.${CC}"
fi

export COMMON_INSTALL_DIR=${BUILD_DIR}/install
export COMMON_BUILD_TYPE=Debug
export EXTERNALS_DIR=${ROOT_DIR}/externals
mkdir -p ${BUILD_DIR}

${SCRIPT_DIR}/prepare_externals.sh

cd ${BUILD_DIR}
cmake .. -DCMAKE_INSTALL_PREFIX=${COMMON_INSTALL_DIR} \
    -DCMAKE_BUILD_TYPE=Debug -DUBLOX_GEN_TEST=ON \
    -DUBLOX_GEN_TOOLS=ON  -DUBLOX_GEN_SWIG=ON -DUBLOX_GEN_EMSCRIPTEN=ON \
    -DUBLOX_BUILD_EXAMPLES=ON "$@"

procs=$(nproc)
if [ -n "${procs}" ]; then
    procs_param="--parallel ${procs}"
fi

cmake --build ${BUILD_DIR} --config ${COMMON_BUILD_TYPE} --target install ${procs_param}
