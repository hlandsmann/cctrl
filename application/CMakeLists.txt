
add_executable(cctrl.elf)
target_sources(cctrl.elf
    PRIVATE
        src/cctrl.cpp
)
target_link_libraries(
    cctrl.elf
    PRIVATE
        ${CMAKE_PROJECT_NAME}_warnings
        utl::utl
        serial::serial
        memman::memman
)
target_compile_features(cctrl.elf PRIVATE cxx_std_20)

add_custom_target(
    cctrl.hex ALL DEPENDS cctrl.elf
    COMMAND ${CMAKE_OBJCOPY}
        -O ihex -R .eeprom cctrl.elf cctrl.hex
    COMMENT
        "Creating uploadable binary cctrl.hex"
)

set_target_properties(
    cctrl.hex
    PROPERTIES
        OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/cctrl.hex
)

add_custom_target(
    print_size ALL
    COMMAND
        ${AVRSIZE} --format=berkeley ${CMAKE_CURRENT_BINARY_DIR}/cctrl.elf
    DEPENDS
        cctrl.elf
)
add_custom_target(
    flashbin
    COMMAND
        ${AVRDUDE} -q -V -p atmega328p
        -C /etc/avrdude.conf -D -c arduino
        -b 115200 -P /dev/ttyACM0
		-U flash:w:$<TARGET_PROPERTY:cctrl.hex,OUTPUT_FILE>:i
    DEPENDS
        cctrl.hex print_size
)
