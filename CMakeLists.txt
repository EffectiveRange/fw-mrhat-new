# todo check
# Set the correct object file extension for macOS
# set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)
# set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
# set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)
# set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

# Set the correct object file extension for macOS

#end


cmake_minimum_required(VERSION 3.22)
# Enable verbose output
set(CMAKE_VERBOSE_MAKEFILE ON)


# Setup compiler settings
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
message("Build type: "              ${CMAKE_BUILD_TYPE})
include(cmake/toolchain_pic.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE cmake/overrides.cmake)

#
# Core project settings
#
project(FW-MR-HAT)          # Modified

# enable_language(C CXX ASM)
enable_language(C ASM)
# set(CMAKE_C_OUTPUT_EXTENSION .p1)
# set(CMAKE_CXX_OUTPUT_EXTENSION .p1)
# set(CMAKE_C_OUTPUT_EXTENSION "p1")
# set(CMAKE_CXX_OUTPUT_EXTENSION "p1")
#
# Core MCU flags, CPU, instruction set and FPU setup
# Needs to be set properly for your MCU
#
set(CPU_PARAMETERS
    # -mthumb

    # # This needs attention to properly set for used MCU
    # -mcpu=cortex-m7                 # Modified
    # -mfpu=fpv5-d16                  # Modified
    # -mfloat-abi=hard                # Modified
)

# Set linker script
set(linker_script_SRC               ${PROJ_PATH}/STM32H735IGKX_FLASH.ld) # Modified
set(EXECUTABLE                      ${CMAKE_PROJECT_NAME})

set(SRC_DIR "src/")
set(SRC_PLAT_DIR "src/platform/")
set(SRC_EXP_DIR "src/experiments/")

message("CPP_FLAGS src: " ${CPP_FLAGS})

#
# List of source files to compile
#
set(sources_SRCS                    # Modified
    ${SRC_DIR}/i2c_app.c
    ${SRC_DIR}/led_ctrl.c
    ${SRC_DIR}/onoff.c
    ${SRC_DIR}/pi_mgr.c
    ${SRC_DIR}/power_mgr.c
    ${SRC_DIR}/tasks.c
    ${SRC_DIR}/timers.c
    ${SRC_DIR}/main.c


    ${SRC_PLAT_DIR}/gpio.c
    ${SRC_PLAT_DIR}/pwm.c
    
    ${SRC_PLAT_DIR}/clock.c
    ${SRC_PLAT_DIR}/config_bits.c
    ${SRC_PLAT_DIR}/i2c1_client.c
    ${SRC_PLAT_DIR}/i2c1_multi.c
    ${SRC_PLAT_DIR}/i2c1.c
    ${SRC_PLAT_DIR}/interrupt.c
    ${SRC_PLAT_DIR}/mvio.c
    ${SRC_PLAT_DIR}/system.c
    ${SRC_PLAT_DIR}/tmr0.c
    ${SRC_PLAT_DIR}/tmr1.c

    ${SRC_EXP_DIR}/exp1_low_power_test.c
)
set_source_files_properties(${sources_SRCS} PROPERTIES COMPILE_FLAGS "${CPP_FLAGS}")
# there is no asm source currently
set_source_files_properties(${ASM_SOURCES} PROPERTIES COMPILE_FLAGS "${ASM_FLAGS}")

# Include directories
#
set(include_path_DIRS               # Modified
    ${SRC_DIR}
    ${SRC_PLAT_DIR}
)

#
# Symbols definition
#
set(symbols_SYMB                    # Modified
    # "DEBUG"
    # "STM32H735xx"
    # "USE_FULL_LL_DRIVER"
    # "HSE_VALUE=25000000"
)

# Executable files
add_executable(${EXECUTABLE} ${sources_SRCS})

# Include paths
target_include_directories(${EXECUTABLE} PRIVATE ${include_path_DIRS})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE ${symbols_SYMB})


# Compiler options
# target_compile_options(${EXECUTABLE} PRIVATE
#     ${CPU_PARAMETERS}
#     # -Wall
#     # -Wextra
#     # -Wpedantic
#     # -Wno-unused-parameter
#     # # Full debug configuration
#     # -Og -g3 -ggdb
# )

# Linker options
message("LINLKER_FLAGS: " ${LINLKER_FLAGS})
target_link_options(${EXECUTABLE} PRIVATE
    ${LINLKER_FLAGS}
    # -T${linker_script_SRC}
    # ${CPU_PARAMETERS}
    # -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    # --specs=nosys.specs
    # -u _printf_float                # STDIO float formatting support
    # -Wl,--start-group
    # -lc
    # -lm
    # -lstdc++
    # -lsupc++
    # -Wl,--end-group
    # -Wl,--print-memory-usage
)

# Execute post-build to print size
set(image_name "default.elf")
set(output_dir "out")

# add_custom_command(
#       TARGET ${EXECUTABLE} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E make_directory ${output_dir}
#     COMMAND ${CMAKE_COMMAND} -E copy ${image_name} ${output_dir}/${image_name}
# )

# add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
#     COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
# )

# # Convert output to hex and binary
# add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
#     COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
# )

# # Convert to bin file -> add conditional check?
# add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
#     COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
# )
# if(EXISTS ${OVERRIDES_CMAKE})
# endif()