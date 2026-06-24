# Demo for CMOC/CoCo port of FreeRTOS
#   Author: Boisy Pitre
#
SRCDIR=../..
RTOS_BASEDIR=$(SRCDIR)/Source
COMMON_DIR=../Common
CPU=turbo9

DEVC_PREFIX=
CC=$(DEVC_PREFIX)cmoc
MAME ?= mame
MAME_MACHINE ?= coco2b
MAME_AUTOBOOT_DELAY ?= 2
MAME_AUTOBOOT_COMMAND ?= RUN"*"\n

ifeq ($(AUTOBOOT),0)
MAME_AUTOBOOT_ARGS =
else
MAME_AUTOBOOT_ARGS = -autoboot_delay $(MAME_AUTOBOOT_DELAY) -autoboot_command '$(MAME_AUTOBOOT_COMMAND)'
endif

CPPFLAGS+=-I. -I$(COMMON_DIR) -I$(COMMON_DIR)/asm-$(CPU)/arch-coco -I../../Source/portable/CMOC/CoCo \
  -I$(RTOS_BASEDIR)/include -DCMOC_COCO

CFLAGS+=--decb -D$(CPU) -Wno-const -DFREERTOS -DCMOC_FREERTOS

RTOS_SRCS = $(RTOS_BASEDIR)/portable/CMOC/CoCo/port.c \
  $(RTOS_BASEDIR)/portable/MemMang/heap_2.c \
  $(RTOS_BASEDIR)/list.c \
  $(RTOS_BASEDIR)/tasks.c

OBJS=main.o startup.o $(RTOS_SRCS:.c=.o)

all: main.obj

main.obj: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

startup.o: $(COMMON_DIR)/startup.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) *.obj *.dsk *.list *.map *.link

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
