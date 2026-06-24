# Demo for CLANG/Turbo9 port of FreeRTOS
#   Author: Boisy Pitre
#
SRCDIR=../..
RTOS_BASEDIR=$(SRCDIR)/Source
COMMON_DIR=../Common
COMPILER_COMMON=$(COMMON_DIR)/CLANG
CPU=turbo9

LLVM6809_ROOT?=/Volumes/Lagniappe/llvm/llvm-mc6809/llvm/build
LLVM6809_BINDIR?=$(LLVM6809_ROOT)/bin
LLVM6809_RTDIR?=$(LLVM6809_ROOT)/lib/Target/MC6809/Runtime

CC=$(LLVM6809_BINDIR)/clang
OBJCOPY=$(LLVM6809_BINDIR)/llvm-objcopy

CPPFLAGS+=-I. -I$(COMPILER_COMMON) -I$(COMMON_DIR) -I$(COMMON_DIR)/asm-$(CPU)/arch-turbo9 -I../../Source/portable/CLANG/Turbo9 \
  -I$(RTOS_BASEDIR)/include -DCLANG_TURBO9

CFLAGS+=-Oz -ffreestanding -fno-builtin -Wno-implicit-function-declaration \
  -D$(CPU) -DFREERTOS

LDFLAGS+=-nostdlib -Wl,--image-base=0 -Wl,--no-gc-sections -Wl,-e,main \
  -Wl,-Ttext=0x2800 -L$(LLVM6809_RTDIR)
LDLIBS+=-lmc6809rt

BUILD_DIR=build-clang
OBJS=$(BUILD_DIR)/main.o \
  $(BUILD_DIR)/startup.o \
  $(BUILD_DIR)/llvm-support.o \
  $(BUILD_DIR)/port.o \
  $(BUILD_DIR)/heap_2.o \
  $(BUILD_DIR)/list.o \
  $(BUILD_DIR)/tasks.o

all: freertos_baseline.img
	@ls -l freertos_baseline.obj

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/main.o: main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/startup.o: $(COMMON_DIR)/startup.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/llvm-support.o: llvm-support.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/port.o: $(RTOS_BASEDIR)/portable/CLANG/Turbo9/port.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/heap_2.o: $(RTOS_BASEDIR)/portable/MemMang/heap_2.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/list.o: $(RTOS_BASEDIR)/list.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/tasks.o: $(RTOS_BASEDIR)/tasks.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

freertos_baseline.obj: $(OBJS)
	$(CC) $(LDFLAGS) -o freertos_baseline.elf $^ $(LDLIBS)
	$(OBJCOPY) -O binary freertos_baseline.elf $@

freertos_baseline.img: freertos_baseline.obj lastpage
	dd if=/dev/zero of=zero.tmp bs=10240 count=1
	cat zero.tmp freertos_baseline.obj > $@.tmp
	os9 padrom -c=0 65280 $@.tmp
	cat $@.tmp lastpage > $@

lastpage: lastpage.asm
	lwasm lastpage.asm --raw -olastpage

run: freertos_baseline.img
	/Volumes/Lagniappe/turbo-shelf/hyper9/.build/arm64-apple-macosx/release/hyper9-cmd freertos_baseline.img

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) *.obj *.elf *.img *.rom *.tmp *.list *.map *.link lastpage
