#include <serial.h>
#include <stdint.h>
#include <iostream>

auto Serial::tx(const char* first, const char* last, bool /*progmem*/) -> uint8_t {
    std::string_view sv(first, last);
    std::cout << sv;
    return sv.size();
}

auto Serial::tx(const uint8_t* first, const uint8_t* last, bool progmem) -> uint8_t {
    return tx((const char*)(first), (const char*)(last), progmem);
}

auto Serial::tx_spaceLeft() -> uint8_t { return 60; }
