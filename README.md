# FreeRTOS for Turbo9, 6809, and CoCo

This repository contains a focused FreeRTOS port for CMOC-based 6809-family
targets:

- Turbo9 on the Hyper9 simulator
- Tandy Color Computer / CoCo-compatible CMOC builds

The repository is not a Git fork of upstream FreeRTOS. It is a focused port
repo built from a FreeRTOS source snapshot plus local CMOC ports and demos.

## Layout

- `FreeRTOS/Source`: FreeRTOS kernel source used by the demos.
- `FreeRTOS/Source/portable/CMOC/Turbo9`: Turbo9 CMOC port layer.
- `FreeRTOS/Source/portable/CMOC/CoCo`: CoCo CMOC port layer.
- `FreeRTOS/Demo/Turbo9_CMOC`: Hyper9/Turbo9 demo.
- `FreeRTOS/Demo/CoCo_CMOC`: CoCo CMOC demo.

## Build and Run

Build and run the Turbo9 Hyper9 demo:

```sh
cd FreeRTOS/Demo/Turbo9_CMOC
make clean
make freertos_baseline.img
make run
```

Build the CoCo demo:

```sh
cd FreeRTOS/Demo/CoCo_CMOC
make clean
make main.obj
```

Build a CoCo disk image:

```sh
make freertos.dsk
```

## Tooling

Expected tools include:

- `cmoc`
- `lwasm`
- `os9`
- `decb`
- Hyper9 for the Turbo9 simulator run target
