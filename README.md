# RA4M1 CMake Project
CMake Project to build for the Renesas R7FA4M1AB MCU

## Prerequisites
1. cmake >= 3.28.3
2. Ninja >= 1.11.1
3. Python >=3.12
4. arm-none-eabi >= 13.2.1

## Building the image
1. Run `python build.py` to build the `.elf image`

## Flashing the image
1. Requires a SEGGER JLink, use JFlash to program the `.elf` into target device

