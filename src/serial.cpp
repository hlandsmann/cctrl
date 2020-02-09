#include "serial.h"
#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <utl/algorithm.h>
#include <utl/execution.h>
#include <utl/iterator.h>
#include <utl/memory.h>

static uint8_t tx_buffer[64];
static uint8_t rx_buffer[64];

using bufIterator = uint8_t*;

static volatile bufIterator it_txRead  = utl::begin(tx_buffer);
static volatile bufIterator it_txWrite = utl::begin(tx_buffer);

static volatile bufIterator it_rxRead  = utl::begin(rx_buffer);
static volatile bufIterator it_rxWrite = utl::begin(rx_buffer);

void Serial::init() {
    // Set baud rate
    uint16_t UBRR0_value;
    int      multiplier;
    if constexpr (baudrate < 57600) {
        multiplier = 8;
        UCSR0A &= ~(1 << U2X0);  // disable double baudrate bit
    } else {
        multiplier = 4;
        UCSR0A |= (1 << U2X0);  // enable double baudrate bit
    }
    UBRR0_value = (((F_CPU / (multiplier * baudrate)) - 1) / 2);

    UBRR0H = UBRR0_value >> 8;
    UBRR0L = UBRR0_value;

    // enable rx and tx
    UCSR0B |= 1 << RXEN0;
    UCSR0B |= 1 << TXEN0;

    // enable interrupt on complete reception of a byte
    UCSR0B |= 1 << RXCIE0;
}

uint8_t Serial::tx(const char* first, const char* last) {
    const uint8_t* uint8_first = reinterpret_cast<const uint8_t*>(first);
    const uint8_t* uint8_last  = reinterpret_cast<const uint8_t*>(last);
    return tx(uint8_first, uint8_last);
}

uint8_t Serial::tx(const uint8_t* first, const uint8_t* last) {
    if (first == last) /*nothing to copy */
        return 0;

    uint8_t spaceNeeded = utl::distance(first, last);
    uint8_t spaceLeft   = tx_spaceLeft();

    if (spaceNeeded > spaceLeft)
        return 0;

    bufIterator new_it_txWrite = utl::copy(first, last, utl::CycleIt(tx_buffer, it_txWrite)).get();

    /* update it_txWrite atomically */
    {
        utl::ClearInterrupts clear;
        it_txWrite = new_it_txWrite;
    }
    UCSR0B |= (1 << UDRIE0);  // start streaming Tx
    return spaceNeeded;
}

uint8_t Serial::tx_spaceLeft() {
    bufIterator temp_it_txRead;
    { /* because of a possible race condition, clear interrupts, and safe read tx iterator in
         a temporary */
        utl::ClearInterrupts clear;
        temp_it_txRead = it_txRead;
    }
    if ((0 == (UCSR0B & (1 << UDRIE0))) && temp_it_txRead == it_txWrite)
        return utl::size(tx_buffer);
    return utl::distance(utl::CycleIt(tx_buffer, it_txWrite), utl::CycleIt(tx_buffer, temp_it_txRead));
}

ISR(USART_UDRE_vect) {
    UDR0      = *it_txRead;
    it_txRead = (++utl::CycleIt(tx_buffer, it_txRead)).get();

    if (it_txRead == it_txWrite)
        UCSR0B &= ~(1 << UDRIE0);  // stop streaming Tx
}

ISR(USART_RX_vect) {
    *it_rxWrite = UDR0;
    Serial::tx(it_rxWrite, it_rxWrite + 1);
    it_rxWrite = (++utl::CycleIt(rx_buffer, it_rxWrite)).get();

    if (it_rxWrite == it_rxRead)
        UCSR0B &= ~(1 << RXCIE0);  // stop streaming Rx
}
