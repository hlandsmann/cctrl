cmake_minimum_required(VERSION 3.5)
project(cctrl)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(UNUSED_OPTION "-x assembler-with-cpp -fno-devirtualize")


set(GCC_INCLUDES "/usr/avr/include")
set(PROJECT_INCLUDES " ${PROJECT_SOURCE_DIR}/src")

add_subdirectory(application)
add_subdirectory(cmake/cctrl_warnings)
add_subdirectory(drivers)
add_subdirectory(libraries)

include(${CMAKE_SOURCE_DIR}/cmake/set_link_options_for_targets.cmake)

add_custom_target(
    flashbin
    COMMAND
        /usr/bin/avrdude -q -V -p atmega328p
        -C /etc/avrdude.conf -D -c arduino
        -b 115200 -P /dev/ttyACM0
		-U flash:w:$<TARGET_PROPERTY:cctrl.hex,OUTPUT_FILE>:i
    DEBENDS
        cctrl.hex
)
