# FreeRTOS CoCo CLANG Demo

This demo builds the CoCo FreeRTOS sample with LLVM-6809/clang and packages it
as a DECB disk image for a Color Computer emulator or compatible target.

The program writes status text directly to the CoCo text screen at `$0400`,
creates a single FreeRTOS task, starts the scheduler, and updates a heartbeat
counter on screen.

## Build

```sh
make freertos.dsk
```

The build produces:

```text
main.obj
freertos.dsk
```

The Makefile links an ELF at load address `$2800`, converts it to a raw binary,
wraps it in a DECB binary container, and copies it to the disk as `FRTOS.BIN`.
It also keeps all clang object files under `build-clang/` so this demo does not
share stale objects with the CMOC demo.

## Run

```sh
make run
```

The run target uses `mame` by default. Set `MAME`, `MAME_MACHINE`,
`MAME_ROM_PATH`, or `AUTOBOOT=0` if your emulator setup differs.

## Toolchain

This port uses LLVM-6809 and the CLANG CoCo portable layer in:

```text
FreeRTOS/Source/portable/CLANG/CoCo
```

The build defaults to:

```text
/Volumes/Lagniappe/llvm/llvm-mc6809/llvm/build
```

Override `LLVM6809_ROOT`, `LLVM6809_BINDIR`, or `LLVM6809_RTDIR` if your
LLVM-6809 build is installed somewhere else.
