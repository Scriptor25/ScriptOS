TEMP_DIR = /tmp/toolchain
SOURCE_DIR = $(TEMP_DIR)/src
BUILD_DIR = $(TEMP_DIR)/build

BINUTILS_BUILD_DIR = $(BUILD_DIR)/binutils
GCC_BUILD_DIR = $(BUILD_DIR)/gcc

BINUTILS_VERSION = 2.43.1
GCC_VERSION = 14.2.0

BINUTILS_NAME = binutils-$(BINUTILS_VERSION)
GCC_NAME = gcc-$(GCC_VERSION)

BINUTILS_FILENAME = $(BINUTILS_NAME).tar.gz
GCC_FILENAME = $(GCC_NAME).tar.gz

BINUTILS_URL = https://ftp.gnu.org/gnu/binutils/$(BINUTILS_FILENAME)
GCC_URL = https://ftp.gnu.org/gnu/gcc/$(GCC_NAME)/$(GCC_FILENAME)

BINUTILS_SOURCE = $(SOURCE_DIR)/$(BINUTILS_FILENAME)
GCC_SOURCE = $(SOURCE_DIR)/$(GCC_FILENAME)

BINUTILS_SOURCE_DIR = $(SOURCE_DIR)/$(BINUTILS_NAME)
GCC_SOURCE_DIR = $(SOURCE_DIR)/$(GCC_NAME)

PREFIX = /usr/local
TARGET = x86_64-elf

.PHONY: all binutils gcc build unpack cleanbuild clean test

all: cleanbuild build test clean

$(BINUTILS_SOURCE):
	@ echo ======== Downloading - Binutils ========
	@ mkdir -p $(SOURCE_DIR)
	@ curl $(BINUTILS_URL) --output $(BINUTILS_SOURCE) --progress-bar

$(GCC_SOURCE):
	@ echo ======== Downloading - GCC =============
	@ mkdir -p $(SOURCE_DIR)
	@ curl $(GCC_URL) --output $(GCC_SOURCE) --progress-bar

$(BINUTILS_SOURCE_DIR): $(BINUTILS_SOURCE)
	@ echo ======== Unpacking - Binutils ==========
	@ tar -xzf $(BINUTILS_SOURCE) -C $(SOURCE_DIR)

$(GCC_SOURCE_DIR): $(GCC_SOURCE)
	@ echo ======== Unpacking - GCC ===============
	@ tar -xzf $(GCC_SOURCE) -C $(SOURCE_DIR)

binutils: $(BINUTILS_SOURCE_DIR)
	@ echo ======== Building - Binutils ===========
	@ (mkdir -p $(BINUTILS_BUILD_DIR);\
	cd $(BINUTILS_BUILD_DIR);\
	$(BINUTILS_SOURCE_DIR)/configure --target=$(TARGET) --prefix=$(PREFIX) --with-sysroot --disable-nls --disable-werror;\
	make -j 14;\
	sudo make install)

gcc: $(GCC_SOURCE_DIR)
	@ echo ======== Building - GCC ================
	@ (mkdir -p $(GCC_BUILD_DIR);\
	cd $(GCC_BUILD_DIR);\
	$(GCC_SOURCE_DIR)/configure --target=$(TARGET) --prefix=$(PREFIX) --disable-nls --enable-languages=c,c++ --without-headers;\
	make -j 14 all-gcc;\
	make -j 14 all-target;\
	sudo make install-gcc;\
	sudo make install-target-libgcc)

build: binutils gcc

unpack: $(BINUTILS_SOURCE_DIR) $(GCC_SOURCE_DIR)

cleanbuild:
	@ echo ======== Cleaning - Build Files ========
	@ rm -rf $(BUILD_DIR)

clean:
	@ echo ======== Cleaning ======================
	@ rm -rf $(TEMP_DIR)
test:
	@ $(TARGET)-gcc --version
