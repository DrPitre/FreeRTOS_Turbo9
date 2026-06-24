# Demo for CMOC/Turbo9 port of FreeRTOS
#   Author: Boisy Pitre
#
SRCDIR=../..
RTOS_BASEDIR=$(SRCDIR)/Source
CPU=turbo9

DEVC_PREFIX=
CC=$(DEVC_PREFIX)cmoc
AS=$(DEVC_PREFIX)as
AR=$(DEVC_PREFIX)ar
OBJDUMP=$(DEVC_PREFIX)lwobjdump

CPPFLAGS+=-i -I. -I./asm-$(CPU)/arch-turbo9 -I../Common/include -I../../Source/portable/CMOC/Turbo9 \
  -I$(RTOS_BASEDIR)/include -DCMOC_TURBO9

CFLAGS+=-i -D$(CPU) -Wno-const -DFREERTOS

RTOS_SRCS = main.c startup.c $(RTOS_BASEDIR)/portable/CMOC/Turbo9/port.c \
  $(RTOS_BASEDIR)/portable/MemMang/heap_2.c \
  $(RTOS_BASEDIR)/list.c \
  $(RTOS_BASEDIR)/tasks.c

OBJS=$(RTOS_SRCS:.c=.o)

all: freertos_baseline.img
	@ls -l freertos_baseline.obj

freertos_baseline.obj: $(OBJS)
	$(CC) -i $(LDFLAGS) --raw -o $@ $^

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
	$(RM) $(OBJS) *.obj *.img *.rom *.tmp *.list *.map *.link lastpage
