set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER_WORKS 1)
# Some default GCC settings
# arm-none-eabi- must be part of path environment
set(TOOLCHAIN_PREFIX                )
set(FLAGS                           "-fdata-sections -ffunction-sections --specs=nano.specs -Wl,--gc-sections")
set(CPP_FLAGS                       "${MP_CC}  ${MP_EXTRA_CC_PRE}  -mcpu=18F16Q20  -c  ${MDFP_PATH}  -fno-short-double  -fno-short-float  -O0  -fasmfile  -maddrqual=ignore  -xassembler-with-cpp  -mwarn=-3  -Wa,-a  -DXPRJ_default=default  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32  -Wl,--data-init  -mno-keep-startup  -mno-download  -mno-default-config-bits  -std=c99  -gdwarf-3  -mstack=compiled:auto:auto:auto  ${INSTRUMENTED_TRACE_OPTIONS}  ${FUNCTION_LEVEL_PROFILING_OPTIONS}")
set(ASM_FLAGS                       "${MP_CC} -c ${MP_EXTRA_AS_PRE} -mcpu=18F16Q20 ${MDFP_PATH} -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=default -msummary=-psect,-class,+mem,-hex,-file -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto ${INSTRUMENTED_TRACE_OPTIONS}")

set(LINLKER_FLAGS -mcpu=18F16Q20  -Wl,-Map=mem.map  -DXPRJ_default=default  -Wl,--defsym=__MPLAB_BUILD=1  ${MDFP_PATH}  -fno-short-double  -fno-short-float  -O0  -fasmfile  -maddrqual=ignore  -xassembler-with-cpp  -mwarn=-3  -Wa,-a  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32  -Wl,--data-init  -mno-keep-startup  -mno-download  -mno-default-config-bits  -std=c99  -gdwarf-3  -mstack=compiled:auto:auto:auto  ${INSTRUMENTED_TRACE_OPTIONS}  ${FUNCTION_LEVEL_PROFILING_OPTIONS}  -Wl,--memorysummary,memoryfile.xml,)
# set(MP_CC_DIR "/opt/microchip/xc8/v2.50")

# Define compiler settings
set(CMAKE_C_COMPILER                ${MP_CC_DIR}/bin/xc8-cc)
set(CMAKE_ASM_COMPILER              ${MP_CC_DIR}/bin/xc8-cc)
set(CMAKE_CXX_COMPILER              ) # do not use c++
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

