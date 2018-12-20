#!/usr/bin/env bash
#
# Exit as soon as an error occurs...
#
set -e
#
# Variables
#
LIBGPGERROR="libgpg-error-1.33"
LIBGCRYPT="libgcrypt-1.8.4"
BUILD_DIR="$(pwd)/build"
#
# Script
#
echo "Recreating a build directory..."
rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}
echo "Building ${LIBGPGERROR}..."
tar xjvf ${LIBGPGERROR}.tar.bz2
cd ${LIBGPGERROR}
./configure CFLAGS='-O3 -m64 -mtune=native -march=native' LDFLAGS='-s -static' --prefix=${BUILD_DIR}
make -j4
make install
cd ..
echo "Building ${LIBGCRYPT}..."
tar xjvf ${LIBGCRYPT}.tar.bz2
cd ${LIBGCRYPT}
./configure CFLAGS='-O3 -m64 -mtune=native -march=native' LDFLAGS='-s -static' --enable-m-guard --with-gpg-error-prefix=${BUILD_DIR} --prefix=${BUILD_DIR}
make -j4
make install
cd ..
echo "Done."
