# Demo for CMOC/CoCo port of FreeRTOS
#   Author: Boisy Pitre
#
SRCDIR=../..
RTOS_BASEDIR=$(SRCDIR)/Source
#include ".././.
CPU=turbo9

DEVC_PREFIX=
CC=$(DEVC_PREFIX)cmoc
AS=$(DEVC_PREFIX)as
AR=$(DEVC_PREFIX)ar
\OBJDUMP=$(DEVC_PREFIX)lwobjdump

CPPFLAGS+=-I. -I./asm-$(CPU)/arch-coco -I../Common/include -I../../Source/portable/CMOC/CoCo \
  -I$(RTOS_BASEDIR)/include -DCMOC_COCO

CFLAGS+=--decb -D$(CPU) -Wno-const -DFREERTOS

CSRCS=main.c startup.c 
#  ../Common/Minimal/PollQ.c

# vectors.c serial.c sci.c ParTest.c gelfunc.c \
#  ../Common/Minimal/flash.c \
#  ../Common/Minimal/dynamic.c \
#  ../Common/Minimal/BlockQ.c \
#  ../Common/Minimal/PollQ.c \
#  ../Common/Minimal/comtest.c \
#  ../Common/Minimal/integer.c \
#  ../Common/Minimal/death.c \

RTOS_OBJS = $(RTOS_BASEDIR)/portable/CMOC/CoCo/port.c \
  $(RTOS_BASEDIR)/portable/MemMang/heap_2.c \
  $(RTOS_BASEDIR)/list.c \
  $(RTOS_BASEDIR)/tasks.c \
  $(RTOS_BASEDIR)/queue.c

#OBJS=$(CSRCS:.c=.o) $(RTOS_OBJS:.c=.o)
OBJS=$(CSRCS:.c=.o) $(RTOS_OBJS:.c=.o)

all:	main.obj

main.obj:	$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ 

clean:
	$(RM) $(OBJS) *.obj

freertos.dsk: main.obj
	decb dskini freertos.dsk
	echo "10 LOADM\"FRTOS.BIN\":EXEC" > /tmp/loader.bas
	decb copy -b -0 -t /tmp/loader.bas freertos.dsk,\AUTOEXEC.BAS
	decb copy -b -0 -t /tmp/loader.bas freertos.dsk,\*.BAS
	decb copy -b -2 main.obj freertos.dsk,FRTOS.BIN

reload:
	osascript -e 'tell application "DriveWire"' -e 'tell server of every document' -e 'reload drive 0' -e 'end tell' -e 'end tell'

emu: freertos.dsk
	mame -inipath ~/mame -cfg_directory ~/mame/cfg coco2b -flop1 $^

demu: freertos.dsk
	mame -inipath ~/mame -cfg_directory ~/mame/cfg coco2b -d -flop1 $^
