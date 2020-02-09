#pragma once
#include <avr/interrupt.h>
#include <stdint.h>
#include <utl/memory.h>

ISR(USART_UDRE_vect);
ISR(USART_RX_vect);
class Serial {
public:
    static void init();
    template <size_t N> static uint8_t tx(const char (&text)[N]) {
        return tx(utl::begin(text), utl::end(text)-1);
    }
    static uint8_t tx(const char* first, const char* last);
    static uint8_t tx(const uint8_t* first, const uint8_t* last);
    static uint8_t tx_spaceLeft();

private:
    constexpr static uint32_t baudrate = 115200;
};
