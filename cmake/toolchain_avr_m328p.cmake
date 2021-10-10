set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-a9)

set(CMAKE_C_COMPILER  avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_ASM_COMPILER avr-gcc)
set(CMAKE_OBJCOPY avr-objcopy)

set(GENERAL_FLAGS "-MMD -D__PROG_TYPES_COMPAT__")
set(ARCHITECTURE_FLAGS "-mmcu=atmega328p -DF_CPU=16000000L -D__AVR_ATmega328P__")
set(OPTIMIZATION_OPTIONS "-ffunction-sections -fdata-sections ")
set(LINKER_FLAGS_INIT "-Wl,-Map=output.map -Wl,--gc-sections,--relax ")

#set(CMAKE_EXE_LINKER_FLAGS "-specs=nosys.specs -T ${PROJECT_SOURCE_DIR}/STM32F412CGUx_FLASH.ld")

string(CONCAT COMPILER_FLAGS_INIT
       " ${GENERAL_FLAGS}"
       " ${ARCHITECTURE_FLAGS}"
       " ${OPTIMIZATION_OPTIONS}"
)

set(CMAKE_C_FLAGS_INIT "${COMPILER_FLAGS_INIT}")
set(CMAKE_CXX_FLAGS_INIT "-fno-threadsafe-statics ${CMAKE_C_FLAGS_INIT}")

set(CMAKE_C_FLAGS_DEBUG_INIT "-O1 -g3")
set(CMAKE_C_FLAGS_MINSIZEREL_INIT "-Os -flto -Werror -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE_INIT "-O2 -flto -Werror -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG_INIT ${CMAKE_C_FLAGS_DEBUG_INIT})
set(CMAKE_C_FLAGS_MINSIZEREL_INIT ${CMAKE_C_FLAGS_MINSIZEREL_INIT})
set(CMAKE_CXX_FLAGS_RELEASE_INIT ${CMAKE_C_FLAGS_RELEASE_INIT})
set_property(GLOBAL
             PROPERTY
                 GLOBAL_LINK_OPTIONS
                     LINKER:-Map=output.map
                     LINKER:--gc-sections,--relax
                     -u _printf_float
                     -lm
                     -flto
                     -ffreestanding
                     -fpack-struct
                     -fno-split-wide-types
)
