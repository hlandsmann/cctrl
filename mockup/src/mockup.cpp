#define PROGMEM

#include <utl/logger.h>
#include <iostream>
int main() {
    print("hello world", uint(8), int(8));
    print("\n");

    const char* ptr = "abc";
    print("pointer: ", ptr);
    print("\n\n");

    using Base = utl::logger::Base;
    // Serial::tx(utl::logger::itoa<uint16_t, Base::hex>(reinterpret_cast<uint16_t>(ptr)));
    uint8_t test = 10;
    utl::logger::printSingleValue<uint8_t, Base::dec>(test);
    print("\n");
    utl::logger::printSingleValue<uint8_t, Base::hex>(11);
    print("\n");
    utl::logger::printSingleValue<int8_t, Base::hex>(12);
    print("\n");
    utl::logger::printSingleValue<int16_t, Base::dec>(12);
    print("\n");
    utl::logger::printSingleValue<uint16_t, Base::dec>(13);
    print("\n");
    return 0;
}
