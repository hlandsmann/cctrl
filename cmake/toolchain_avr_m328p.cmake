set(CMAKE_SYSTEM_NAME Generic)

find_program(CMAKE_ASM_COMPILER avr-gcc)
find_program(CMAKE_CXX_COMPILER avr-g++)
find_program(CMAKE_C_COMPILER avr-gcc)
find_program(CMAKE_AR avr-ar)
find_program(CMAKE_STRIP avr-strip)
find_program(CMAKE_OBJCOPY avr-objcopy)
find_program(CMAKE_OBJDUMP avr-objdump)
find_program(AVRSIZE avr-size)
find_program(AVRDUDE avrdude)

#ToDo: putting the avr includes that way, is probably not the best solution...
set(AVR_INCLUDES "-I/usr/avr/include")

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

set(CMAKE_C_FLAGS_INIT "${COMPILER_FLAGS_INIT} ${AVR_INCLUDES}")
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
