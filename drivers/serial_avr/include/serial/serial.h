#pragma once
#include <avr/interrupt.h>
#include <stdint.h>
#include <utl/array.h>
#include <utl/memory.h>
#include <string_view>

ISR(USART_UDRE_vect);
ISR(USART_RX_vect);
class Serial {
public:
    static void init();

    template <size_t N> static auto tx(const utl::array_progmem<char, N>& text) -> uint8_t {
        return tx(text.begin(), text.end(), true);
    }
    template <size_t N> static auto tx(const std::array<char, N>& text) -> uint8_t {
        return tx(text.begin(), text.end());
    }
    template <size_t N> static auto tx_p(const std::array<char, N>& text) -> uint8_t {
        return tx(text.begin(), text.end(), true);
    }
    template <size_t N> static auto tx(const char (&text)[N]) -> uint8_t {
        return tx(utl::begin(text), utl::end(text));
    }
    template <size_t N> static auto tx_p(const char (&text)[N]) -> uint8_t {
        return tx(utl::begin(text), utl::end(text), true);
    }

    static auto tx(std::string_view sv) -> uint8_t { return tx(sv.begin(), sv.end(), true); }
    static auto tx(const char* first, const char* last, bool progmem = false) -> uint8_t;
    static auto tx(const uint8_t* first, const uint8_t* last, bool progmem = false) -> uint8_t;
    static auto tx_spaceLeft() -> uint8_t;

private:
    constexpr static uint32_t baudrate = 115200;
};
