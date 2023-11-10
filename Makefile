
PLATFORM := $(shell uname)
NATIVE_PLATFORM := TRUE

include scripts/rules/os.mk

# TARGET:=i2c_oled

# include ../../ch32v003fun/ch32v003fun.mk

# flash : cv_flash
# clean : cv_clean

# SOURCES = src/i2c_oled.c
# SOURCEDIRS = lib/ch32v003fun

TARGET = prime

SYSTEM_LIB = lib/system

DIRS = src
DIRS += lib/vo_lib/src
DIRS += lib/minifb
DIRS += src/patch
DIRS += src/patch/sto
DIRS += src/ui
DIRS += src/fx/reverb
DIRS += src/fx/reverb/freeverb


TARGET_CFLAGS += \
	-I/opt/homebrew/include \
	-Ires/ \
	-g

TARGET_CFLAGS += \
	-DLIBFV3_FLOAT

TARGET_CFLAGS += -Wno-deprecated

TARGET_LDFLAGS += -g -lm -lstdc++ -lpthread -lportaudio
TARGET_LDFLAGS += -L/opt/homebrew/lib/ 
# TARGET_LDFLAGS += -lminifb -Llib/minifb 
# TARGET_LDFLAGS +=-Llib/minifb
# ifeq ($(TARGET_CPU),aarch64)
# TARGET_LDFLAGS += -lminifb-macos-aarch64 
# else
# TARGET_LDFLAGS += -lminifb-macos-x86_64
# endif
ifeq ($(TARGET_OS),MacOS)
TARGET_LDFLAGS += -framework AppKit -framework MetalKit -framework Metal
endif
	#  -I/opt/homebrew/include -I/Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/usr/include

	# -g -Os -flto -ffunction-sections \
	# -static-libgcc \
	# -march=rv32ec \
	# -mabi=ilp32e \
	# -I/usr/include/newlib \
	# -I$(CH32V003FUN)/../extralibs \
	# -I$(CH32V003FUN) \
	# -nostdlib \
	# -I. -Wall $(EXTRA_CFLAGS)

# include scripts/build_rules.inc
include scripts/rules/compile.inc



# tdaw:	serial.c
# 	$(CC) -o '$@' '$<' -lm -lportaudio -lpthread -L/opt/homebrew/lib/ 
# clean:
# 	rm tdaw

os:
	@echo OS:      $(PLATFORM)
	@echo CFLAGS:  $(CFLAGS)

all: build

.PHONY: os clean