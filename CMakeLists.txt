cmake_minimum_required(VERSION 3.20)
project(ra4m1_project C ASM)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/script/fsp.ld)

file(GLOB_RECURSE SOURCES
    "src/hal_entry.c"
    "ra_gen/*.c"
    "fsp/src/*.c"
    "ra/**/*.c"
)

add_executable(${PROJECT_NAME}.elf ${SOURCES})

target_include_directories(${PROJECT_NAME}.elf PRIVATE
    ${CMAKE_SOURCE_DIR}/ra/fsp/inc
    ${CMAKE_SOURCE_DIR}/ra/fsp/inc/api
    ${CMAKE_SOURCE_DIR}/ra/fsp/inc/instances
    ${CMAKE_SOURCE_DIR}/ra/fsp/src/bsp/mcu/all
    ${CMAKE_SOURCE_DIR}/ra/fsp/src/bsp/mcu/ra4m1
    ${CMAKE_SOURCE_DIR}/ra_gen
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/ra_cfg/fsp_cfg
    ${CMAKE_SOURCE_DIR}/ra_cfg/fsp_cfg/bsp
    ${CMAKE_SOURCE_DIR}/ra/arm/CMSIS_6/CMSIS/Core/Include

)

target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
)

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -g          # Generate debug info
    -O0         # Disable optimization (makes debugging easier)
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -L${CMAKE_SOURCE_DIR}/script
    -T${LINKER_SCRIPT}
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary
            ${PROJECT_NAME}.elf
            ${PROJECT_NAME}.bin
)
