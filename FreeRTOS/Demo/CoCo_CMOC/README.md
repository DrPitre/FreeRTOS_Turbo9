# FreeRTOS CoCo CMOC Demo

This demo builds the CoCo FreeRTOS sample with CMOC and packages it as a DECB
disk image for a Color Computer emulator or compatible target.

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

`main.obj` is copied to the disk as `FRTOS.BIN`, and the disk also includes an
`AUTOEXEC.BAS` loader that runs the binary.

## Run

```sh
make run
```

The run target uses `mame` by default. Set `MAME`, `MAME_MACHINE`,
`MAME_ROM_PATH`, or `AUTOBOOT=0` if your emulator setup differs.

## Toolchain

This port uses the CMOC 6809 C compiler and the shared CMOC CoCo portable layer
in:

```text
FreeRTOS/Source/portable/CMOC/CoCo
```

It uses `decb` to create and populate the CoCo disk image.
