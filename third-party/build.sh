#!/usr/bin/env bash
#
# Exit as soon as an error occurs...
#
set -e
CWD="$(pwd)"
#
# Variables
#
BOTAN="botan-2.8.0"
BUILD_DIR="${CWD}/build"
#
# Script
#
echo "Recreating a build directory..."
rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}
echo "Building ${BOTAN}..."
tar xzvf ${BOTAN}.tar.gz
cd ${BOTAN}
./configure.py --prefix=${BUILD_DIR}
make -j4
make install
cd ${CWD}
echo "Done."
