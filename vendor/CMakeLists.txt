cmake_minimum_required(VERSION 3.20)

# ****************************
# SEGGER RTT Library
# ****************************
add_library(SEGGER_RTT STATIC
    ${CMAKE_CURRENT_LIST_DIR}/SEGGER_RTT/RTT/SEGGER_RTT.c
    ${CMAKE_CURRENT_LIST_DIR}/SEGGER_RTT/RTT/SEGGER_RTT_printf.c
)

target_include_directories(SEGGER_RTT
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/SEGGER_RTT/RTT
        ${CMAKE_CURRENT_LIST_DIR}/SEGGER_RTT/Config
)

# Create an alias for easier linking
add_library(SEGGER::RTT ALIAS SEGGER_RTT)

# ****************************
# FreeRTOS Configuration Interface
# ****************************
# This interface points FreeRTOS to your FreeRTOSConfig.h
add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../ra_cfg
)

# ****************************
# FreeRTOS Kernel
# ****************************
set(FREERTOS_HEAP 4 CACHE STRING "Select FreeRTOS heap implementation (0-heap_1, 1-heap_2, ... 4-heap_5)")

set(FREERTOS_PORT "GCC_ARM_CM4F" CACHE STRING "FreeRTOS port")

add_subdirectory(FreeRTOS-Kernel)
