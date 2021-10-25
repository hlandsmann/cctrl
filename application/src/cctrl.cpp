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
    static_assert(uint16_t(0x1010) == utl::BitPattern("---1------------"));
    print("\n\r");

    print("Hello World!\n\r");

    print("Best Number: {}\n\r", 42);

    // print("{}",42);
    print("{:d}{}{}{}",42,42,42,42);

    while (Serial::tx_spaceLeft() < 60)
        ;
    print("\n\r");
    print("\n\r");


    for (int i = 0; i < 256; i++) {
        while (Serial::tx_spaceLeft() < 60)
            ;
        uint16_t *ptr = new uint16_t[10];
        if (ptr != nullptr) {
            // print("\n\rPtr: {}", ptr);

        } else
            break;
    }

    constexpr uint8_t val = 0x20;
    DDRB = DDRB | val;

    while (true) {
        wdt_reset();
        // toggle();
        // if(TCNT0>250){
        //     toggle();
        //     TCNT0 = 0;
        // }
    }
    return 0;
}
