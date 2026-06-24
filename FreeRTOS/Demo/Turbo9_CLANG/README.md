# FreeRTOS Turbo9 CLANG Demo

This demo builds the Turbo9 FreeRTOS sample with LLVM-6809/clang and packages
it as a 64K ROM image for Hyper9.

The program writes status text to the Hyper9 terminal device at `$FF00`,
creates a single FreeRTOS task, starts the scheduler, and prints periodic
heartbeat messages from that task.

## Build

```sh
make
```

The default target builds:

```text
freertos_baseline.img
```

The Makefile keeps all clang object files under `build-clang/` so this demo
does not share stale objects with the CMOC demo.

## Run

```sh
make run
```

or from the shelf root:

```sh
./hyper9/.build/release/hyper9-cmd FreeRTOS_Turbo9/FreeRTOS/Demo/Turbo9_CLANG/freertos_baseline.img
```

## Toolchain

This port uses LLVM-6809 and the CLANG Turbo9 portable layer in:

```text
FreeRTOS/Source/portable/CLANG/Turbo9
```

The build defaults to:

```text
/Volumes/Lagniappe/llvm/llvm-mc6809/llvm/build
```

Override `LLVM6809_ROOT`, `LLVM6809_BINDIR`, or `LLVM6809_RTDIR` if your
LLVM-6809 build is installed somewhere else.
