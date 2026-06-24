# Demo for CLANG/CoCo port of FreeRTOS
#   Author: Boisy Pitre
#
SRCDIR=../..
RTOS_BASEDIR=$(SRCDIR)/Source
COMMON_DIR=../Common
CPU=turbo9
BUILD_DIR=build-clang
LOAD_ADDR?=0x2800

LLVM6809_ROOT?=/Volumes/Lagniappe/llvm/llvm-mc6809/llvm/build
LLVM6809_BINDIR?=$(LLVM6809_ROOT)/bin
LLVM6809_RTDIR?=$(LLVM6809_ROOT)/lib/Target/MC6809/Runtime

CC=$(LLVM6809_BINDIR)/clang
OBJCOPY=$(LLVM6809_BINDIR)/llvm-objcopy
MAME ?= mame
MAME_MACHINE ?= coco2b
MAME_AUTOBOOT_DELAY ?= 2
MAME_AUTOBOOT_COMMAND ?= RUN"*"\n

ifeq ($(AUTOBOOT),0)
MAME_AUTOBOOT_ARGS =
else
MAME_AUTOBOOT_ARGS = -autoboot_delay $(MAME_AUTOBOOT_DELAY) -autoboot_command '$(MAME_AUTOBOOT_COMMAND)'
endif

CPPFLAGS+=-I. -I$(COMMON_DIR) -I$(COMMON_DIR)/asm-$(CPU)/arch-coco -I../../Source/portable/CLANG/CoCo \
  -I$(RTOS_BASEDIR)/include -DCLANG_COCO

CFLAGS+=-Oz -ffreestanding -fno-builtin -Wno-implicit-function-declaration \
  -D$(CPU) -DFREERTOS

LDFLAGS+=-nostdlib -Wl,--image-base=0 -Wl,--no-gc-sections \
  -Wl,-Tcoco-clang.ld -L$(LLVM6809_RTDIR)
LDLIBS+=-lmc6809rt

OBJS=$(BUILD_DIR)/main.o \
  $(BUILD_DIR)/startup.o \
  $(BUILD_DIR)/llvm-support.o \
  $(BUILD_DIR)/port.o \
  $(BUILD_DIR)/heap_2.o \
  $(BUILD_DIR)/list.o \
  $(BUILD_DIR)/tasks.o

all: main.obj

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/main.o: main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/startup.o: $(COMMON_DIR)/startup.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/llvm-support.o: llvm-support.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/port.o: $(RTOS_BASEDIR)/portable/CLANG/CoCo/port.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/heap_2.o: $(RTOS_BASEDIR)/portable/MemMang/heap_2.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/list.o: $(RTOS_BASEDIR)/list.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/tasks.o: $(RTOS_BASEDIR)/tasks.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

main.elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

main.bin: main.elf
	$(OBJCOPY) -O binary $< $@

main.obj: main.bin
	perl -e 'use strict; use warnings; my ($$in,$$out,$$addr)=@ARGV; open my $$fh,"<:raw",$$in or die $$!; local $$/; my $$data=<$$fh>; close $$fh; my $$load=oct($$addr); my $$len=length($$data); open my $$of,">:raw",$$out or die $$!; print $$of pack("Cnn",0,$$len,$$load), $$data, pack("Cnn",255,0,$$load); close $$of;' $< $@ $(LOAD_ADDR)

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r cfg
	$(RM) *.obj *.elf *.bin *.dsk *.list *.map *.link

freertos.dsk: main.obj
	decb dskini freertos.dsk
	printf "10 CLS\n20 PRINT \"LOADING FREERTOS COCO\"\n30 LOADM\"FRTOS.BIN\":EXEC\n" > /tmp/loader.bas
	decb copy -b -0 -t /tmp/loader.bas freertos.dsk,\AUTOEXEC.BAS
	decb copy -b -0 -t /tmp/loader.bas freertos.dsk,\*.BAS
	decb copy -b -2 main.obj freertos.dsk,FRTOS.BIN

reload:
	osascript -e 'tell application "DriveWire"' -e 'tell server of every document' -e 'reload drive 0' -e 'end tell' -e 'end tell'

run emu: freertos.dsk
	$(MAME) -nothrottle -skip_gameinfo -rompath $(MAME_ROM_PATH) -window $(MAME_MACHINE) -flop1 $^ $(MAME_AUTOBOOT_ARGS)

run_debug demu: freertos.dsk
	$(MAME) -nothrottle -skip_gameinfo -rompath $(MAME_ROM_PATH) -window $(MAME_MACHINE) -flop1 $^ $(MAME_AUTOBOOT_ARGS) -d
