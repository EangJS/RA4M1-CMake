cmake_minimum_required(VERSION 3.14)

# Create SEGGER RTT library target
add_library(SEGGER_RTT STATIC
    ${CMAKE_CURRENT_LIST_DIR}/SEGGER_RTT/RTT/SEGGER_RTT.c
    ${CMAKE_CURRENT_LIST_DIR}/SEGGER_RTT/RTT/SEGGER_RTT_printf.c
)

# Public include directories
target_include_directories(SEGGER_RTT
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/SEGGER_RTT/RTT
        ${CMAKE_CURRENT_LIST_DIR}/SEGGER_RTT/Config
)

add_library(SEGGER::RTT ALIAS SEGGER_RTT)
