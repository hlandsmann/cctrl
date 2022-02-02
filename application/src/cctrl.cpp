#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <utl/bitpattern.h>

#include <memman/memman.h>
#include <serial/serial.h>
#include <utl/execution.h>
#include <utl/logger.h>
#include <algorithm>
#include <ranges>

int main() {
    Serial::init();
    memman::initMemory();

    print("Hello World! {:d}, {}\n\r", int8_t(-42), int16_t(-42));


    while (Serial::tx_spaceLeft() < 60)
        ;

    int *ptr = new int(42);

    print("val: \"{}\" bin: {:b}, dec: {:d}\n\r", ptr, int16_t(42), int16_t(42));
    print("I drink {:X}\n\r", 0xC0FFEE);
    for (int i = 0; i < 256; i++) {
        while (Serial::tx_spaceLeft() < 60)
            ;
        uint16_t *ptr = new uint16_t[10];
        print("{:x}\n", ptr);

        if (ptr != nullptr) {
            print("\n\rPtr: {}", ptr);

        } else
            break;
    }

    constexpr uint8_t val = 0x20;
    DDRB = DDRB | val;

    while (true) {
        wdt_reset();
    }
    return 0;
}
