#!/bin/sh

BINUTILS_VERSION="2.43.1"
GCC_VERSION="14.2.0"

BINUTILS_NAME="binutils-$BINUTILS_VERSION"
GCC_NAME="gcc-$GCC_VERSION"

BINUTILS_FILENAME="$BINUTILS_NAME.tar.gz"
GCC_FILENAME="$GCC_NAME.tar.gz"

BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/$BINUTILS_FILENAME"
GCC_URL="https://ftp.gnu.org/gnu/gcc/$GCC_NAME/$GCC_FILENAME"

TMP="/tmp/toolchain"
SRC="$TMP/src"
BINUTILS_BUILD="$TMP/build/binutils"
GCC_BUILD="$TMP/build/gcc"

PREFIX="/usr/local"
TARGET="i686-elf"

# download and unpack binutils and gcc
mkdir -p $SRC
cd $SRC

curl $BINUTILS_URL --output $BINUTILS_FILENAME
tar -xzf $BINUTILS_FILENAME -C $SRC

curl $GCC_URL --output $GCC_FILENAME
tar -xzf $GCC_FILENAME -C $SRC

# build binutils
mkdir -p $BINUTILS_BUILD
cd $BINUTILS_BUILD
$SRC/$BINUTILS_NAME/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j 8
sudo make install

# build gcc
mkdir -p $GCC_BUILD
cd $GCC_BUILD
$SRC/$GCC_NAME/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make -j 8 all-gcc
make -j 8 all-target
sudo make install-gcc
sudo make install-target-libgcc

# clean up
rm -rf $TMP

# test
$TARGET-gcc --version
