cmake_minimum_required(VERSION 3.20)

# ****************************
# Project Definition
# ****************************
project(ra4m1_project LANGUAGES C ASM)

# ****************************
# Linker Script & FreeRTOS Port
# ****************************
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/script/fsp.ld")

# ****************************
# Source Files
# ****************************
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/hal_entry.c"
    "${CMAKE_SOURCE_DIR}/ra_gen/*.c"
    "${CMAKE_SOURCE_DIR}/ra/**/*.c"
)

# ****************************
# Add Executable
# ****************************
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# ****************************
# Vendor Subdirectories
# ****************************
add_subdirectory(vendor)

# ****************************
# Include Directories
# ****************************
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    # RA FSP headers
    ${CMAKE_SOURCE_DIR}/ra/fsp/inc
    ${CMAKE_SOURCE_DIR}/ra/fsp/inc/api
    ${CMAKE_SOURCE_DIR}/ra/fsp/inc/instances
    ${CMAKE_SOURCE_DIR}/ra/fsp/src/bsp/mcu/all
    ${CMAKE_SOURCE_DIR}/ra/fsp/src/bsp/mcu/ra4m1

    # Project-specific headers
    ${CMAKE_SOURCE_DIR}/ra_gen
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/ra_cfg/fsp_cfg
    ${CMAKE_SOURCE_DIR}/ra_cfg/fsp_cfg/bsp
    ${CMAKE_SOURCE_DIR}/ra/arm/CMSIS_6/CMSIS/Core/Include

    # FreeRTOS
    vendor/FreeRTOS-Kernel/include
    vendor/FreeRTOS-Kernel/portable/GCC/ARM_CM4F
)

# ****************************
# Compiler Definitions & Options
# ****************************
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    # Add project-specific macros here
)

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -g        # debug symbols
    -Os       # optimize for size
    -Wall     # enable warnings
    -Werror   # treat warnings as errors
)

# ****************************
# Linker Settings
# ****************************
target_link_libraries(${PROJECT_NAME}.elf PRIVATE
    SEGGER::RTT
    freertos_kernel
    freertos_config
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -L${CMAKE_SOURCE_DIR}/script
    -T${LINKER_SCRIPT}
)

# ****************************
# Post-build: Generate HEX
# ****************************
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex
            ${PROJECT_NAME}.elf
            ${PROJECT_NAME}.hex
)
