# FreeRTOS Kernel Snapshot

This directory contains the FreeRTOS kernel source snapshot used by the
Turbo9/6809/CoCo CMOC ports in this repository.

The only included demos are:

- `Demo/Turbo9_CMOC`
- `Demo/CoCo_CMOC`

The only local portable layers are:

- `Source/portable/CMOC/Turbo9`
- `Source/portable/CMOC/CoCo`

This is intentionally smaller than an upstream FreeRTOS distribution. The
FreeRTOS-Plus libraries, AWS demos, generic official demo tree, and upstream
repository metadata have been removed from this port-focused repository.
