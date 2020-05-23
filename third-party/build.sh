#!/usr/bin/env bash
#
# Exit as soon as an error occurs...
#
set -e
#
# Variables
#
CWD="$(pwd)"
BOTAN="botan-2.14.0"
DIST_DIR="${CWD}/../picsou/third-party/build"
#
# Script
#
mkdir -p ${DIST_DIR}
tar xzvf ${BOTAN}.tar.gz
cd ${BOTAN}
./configure.py --prefix=${DIST_DIR}
make -j4
make install
cd ${CWD}
