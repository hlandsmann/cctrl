cmake_minimum_required(VERSION 3.5)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

#set(UNUSED_OPTION "-x assembler-with-cpp -fno-devirtualize")

set(CMAKE_C_COMPILER  avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_ASM_COMPILER avr-gcc)
set(CMAKE_OBJCOPY avr-objcopy)
#set(CMAKE_C_COMPILER_WORKS 1)
#set(CMAKE_CXX_COMPILER_WORKS 1)

set(AVR_WARNINGLEVEL "-Wall -Wextra ")
set(GENERAL_FLAGS "-MMD -D__PROG_TYPES_COMPAT__")
set(AVR_328p_FLAGS "-mmcu=atmega328p -DF_CPU=16000000L -D__AVR_ATmega328P__")
set(OPTIMIZATION_OPTIONS "-ffunction-sections -fdata-sections -O2 -flto ")
set(LINKER_FLAGS "-Wl,-Map=output.map -Wl,--gc-sections,--relax -u _printf_float -lm -flto -ffreestanding -fpack-struct -fno-split-wide-types")

#set(CMAKE_EXE_LINKER_FLAGS "-specs=nosys.specs -T ${PROJECT_SOURCE_DIR}/STM32F412CGUx_FLASH.ld")

set(CMAKE_C_FLAGS "${AVR_WARNINGLEVEL} ${GENERAL_FLAGS} ${AVR_328p_FLAGS} ${OPTIMIZATION_OPTIONS}")
set(CMAKE_CXX_FLAGS "-std=c++17 -pedantic -fno-threadsafe-statics ${CMAKE_C_FLAGS}")
#set(CMAKE_ASM_FLAGS

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS})
set(CMAKE_C_FLAGS_MINSIZEREL ${CMAKE_C_FLAGS})
set(CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS})
set(CMAKE_C_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS_MINSIZEREL ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS})
set(CMAKE_ASM_FLAGS_DEBUG ${CMAKE_ASM_FLAGS})
set(CMAKE_ASM_FLAGS_MINSIZEREL ${CMAKE_ASM_FLAGS})
set(CMAKE_ASM_FLAGS_RELEASE ${CMAKE_ASM_FLAGS})
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO ${CMAKE_ASM_FLAGS})


set(BUILD_TARGET cctrl)
set(BUILD_TARGET_ELF ${BUILD_TARGET}.elf)
set(BUILD_TARGET_BIN ${BUILD_TARGET}.bin)
set(BUILD_TARGET_HEX ${BUILD_TARGET}.hex)

set(GCC_INCLUDES "/usr/avr/include")
set(PROJECT_INCLUDES " ${PROJECT_SOURCE_DIR}/src")

include_directories(${GCC_INCLUDES} ${PROJECT_INCLUDES})

add_executable(${BUILD_TARGET_ELF} src/main.cpp
                                   src/memman.cpp
                                   src/serial.cpp
                                   )
target_link_libraries(${BUILD_TARGET_ELF} "${LINKER_FLAGS}")

add_custom_target(${BUILD_TARGET_BIN} ALL DEPENDS ${BUILD_TARGET_ELF})
add_custom_command(TARGET ${BUILD_TARGET_BIN} DEPENDS ${BUILD_TARGET_ELF}
           COMMAND ${CMAKE_OBJCOPY} -O binary ${BUILD_TARGET_ELF} ${BUILD_TARGET_BIN}
           COMMENT "Creating binary ${BUILD_TARGET_BIN}")

add_custom_target(${BUILD_TARGET_HEX} ALL DEPENDS ${BUILD_TARGET_ELF})
add_custom_command(TARGET ${BUILD_TARGET_HEX} DEPENDS ${BUILD_TARGET_ELF}
           COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom ${BUILD_TARGET_ELF} ${BUILD_TARGET_HEX}
           COMMENT "Creating uploadable binary ${BUILD_TARGET_HEX}")
