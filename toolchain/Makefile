TMP_DIR = /tmp/toolchain
SRC_DIR = $(TMP_DIR)/src
BINUTILS_BUILD = $(TMP_DIR)/build/binutils
GCC_BUILD = $(TMP_DIR)/build/gcc

BINUTILS_VERSION = 2.43.1
GCC_VERSION = 14.2.0

BINUTILS_NAME = binutils-$(BINUTILS_VERSION)
GCC_NAME = gcc-$(GCC_VERSION)

BINUTILS_FILENAME = $(BINUTILS_NAME).tar.gz
GCC_FILENAME = $(GCC_NAME).tar.gz

BINUTILS_URL = https://ftp.gnu.org/gnu/binutils/$(BINUTILS_FILENAME)
GCC_URL = https://ftp.gnu.org/gnu/gcc/$(GCC_NAME)/$(GCC_FILENAME)

BINUTILS_SRC_FILE = $(SRC_DIR)/$(BINUTILS_FILENAME)
GCC_SRC_FILE = $(SRC_DIR)/$(GCC_FILENAME)

BINUTILS_SRC_DIR = $(SRC_DIR)/$(BINUTILS_NAME)
GCC_SRC_DIR = $(SRC_DIR)/$(GCC_NAME)

PREFIX = /usr/local
TARGET = i686-elf

.PHONY: all binutils gcc build unpack clean test

all: clean build test

$(BINUTILS_SRC_FILE):
	@ echo ======== Downloading - Binutils ========
	@ mkdir -p $(SRC_DIR)
	@ curl $(BINUTILS_URL) --output $(BINUTILS_SRC_FILE) --silent

$(GCC_SRC_FILE):
	@ echo ======== Downloading - GCC =============
	@ mkdir -p $(SRC_DIR)
	@ curl $(GCC_URL) --output $(GCC_SRC_FILE) --silent

$(BINUTILS_SRC_DIR): $(BINUTILS_SRC_FILE)
	@ echo ======== Unpacking - Binutils ==========
	@ tar -xzf $(BINUTILS_SRC_FILE) -C $(SRC_DIR)

$(GCC_SRC_DIR): $(GCC_SRC_FILE)
	@ echo ======== Unpacking - GCC ===============
	@ tar -xzf $(GCC_SRC_FILE) -C $(SRC_DIR)

binutils: $(BINUTILS_SRC_DIR)
	@ echo ======== Building - Binutils ===========
	@ (mkdir -p $(BINUTILS_BUILD);\
	cd $(BINUTILS_BUILD);\
	$(BINUTILS_SRC_DIR)/configure --target=$(TARGET) --prefix=$(PREFIX) --with-sysroot --disable-nls --disable-werror;\
	make -j 8;\
	sudo make install)

gcc: $(GCC_SRC_DIR)
	@ echo ======== Building - GCC ================
	@ (mkdir -p $(GCC_BUILD);\
	cd $(GCC_BUILD);\
	$(GCC_SRC_DIR)/configure --target=$(TARGET) --prefix=$(PREFIX) --disable-nls --enable-languages=c,c++ --without-headers;\
	make -j 8 all-gcc;\
	make -j 8 all-target;\
	sudo make install-gcc;\
	sudo make install-target-libgcc)

build: binutils gcc

unpack: $(BINUTILS_SRC_DIR) $(GCC_SRC_DIR)

clean:
	@ echo ======== Cleaning ======================
	@ rm -rf $(TMP_DIR)

test:
	@ $(TARGET)-gcc --version
