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
#include <printf.h>




int main() {
    Serial::init();
    memman::initMemory();
    static_assert(uint16_t(0x1010) == utl::BitPattern("---1------------"));
    print("\n\r");
    // print("my interger {:d} - {:x} - {:o} \n\r", int8_t(-128), int16_t(-2210), uint8_t(8), uint16_t(16));
    // printf("Hello World\n\r");
    print("\n\r\n\r");
    while (Serial::tx_spaceLeft() < 60)
        ;
    print("Hello World\n\r");
    print("\n");

    for (int i = 0; i < 256; i++) {
        while (Serial::tx_spaceLeft() < 60)
            ;
        print("\n\rPtr: ");
       uint16_t *ptr = new uint16_t[10];
        if (ptr != nullptr) {
            // Serial::tx(itoa(reinterpret_cast<uint16_t>(ptr)).string);
            // using Base = utl::logger::Base;
            // // Serial::tx(utl::logger::itoa<uint16_t, Base::hex>(reinterpret_cast<uint16_t>(ptr)));
            // Serial::tx(utl::logger::itoa<decltype(ptr), Base::hex>(ptr));
            // uint8_t test = 10;
            // utl::logger::printSingleValue<uint8_t, Base::dec>(test);
            // utl::logger::printSingleValue<uint8_t, Base::hex>(11);
            // utl::logger::printSingleValue<int8_t, Base::hex>(12);
            // utl::logger::printSingleValue<int16_t, Base::dec>(12);
            // utl::logger::printSingleValue<uint16_t, Base::dec>(13);
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
