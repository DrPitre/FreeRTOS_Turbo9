# FreeRTOS Turbo9 CMOC Demo

This demo builds the Turbo9 FreeRTOS sample with CMOC and packages it as a
64K ROM image for Hyper9.

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

## Run

```sh
make run
```

or from the shelf root:

```sh
./hyper9/.build/release/hyper9-cmd FreeRTOS_Turbo9/FreeRTOS/Demo/Turbo9_CMOC/freertos_baseline.img
```

## Toolchain

This port uses the CMOC 6809 C compiler and the shared CMOC Turbo9 portable
layer in:

```text
FreeRTOS/Source/portable/CMOC/Turbo9
```

It also uses `lwasm` for the final vector page and `os9 padrom` to pad the ROM
image.
