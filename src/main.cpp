#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include "serial.h"
#include "utl/execution.h"
void delayAbit() {
    for (int i = 0; i < 255; i++) {
        while (TCNT0 < 250)
            ;
        // while (TCNT0 > 100)
        //     ;
    }
}

extern void serial_init();
extern void serial_write(uint8_t data);

void timer0_init() {
    // constexpr uint8_t pre0    = (0 << CS02) | (0 << CS01) | (1 << CS00);
    // constexpr uint8_t pre8    = (0 << CS02) | (1 << CS01) | (0 << CS00);
    // constexpr uint8_t pre64   = (0 << CS02) | (1 << CS01) | (1 << CS00);
    // constexpr uint8_t pre256  = (1 << CS02) | (0 << CS01) | (0 << CS00);
    constexpr uint8_t pre1024 = (1 << CS02) | (0 << CS01) | (1 << CS00);

    utl::ClearInterrupts clear;

    // set up timer with no prescaling
    TCCR0A = 0;  // set entire TCCR2A register to 0
    TCCR0B = 0;  // same for TCCR2B
    // initialize counter
    TCNT0 = 0;

    // set compare match register for 2khz increments

    OCR0A = 254;  // = (16*10^6) / (2000*64) - 1 (must be <256)

    // turn on CTC mode
    TCCR0A |= (1 << WGM01);
    // Set CS01 and CS00 bits for 64 prescaler
    TCCR0B |= pre1024;
    // TCCR0B |= (1 << CS01);

    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);
    // sei();

    // set up timer with prescaler = 256
    // TCCR0B |= pre1024;

    // // initialize counter
    // TCNT0 = 0;

    // // enable overflow interrupt
    // TIMSK0 |= (1 << TOIE0);

    // enable global interrupts
    // sei();
}

void toggle() {
    constexpr uint8_t val = 0x20;

    PORTB ^= val;
}

ISR(TIMER0_COMPA_vect) {
    // constexpr uint8_t val = 0x20;
    // toggle();

    // if (i < 127)
    //     PORTB |= val;
    // else
    // PORTB &= ~val;
}
// ISR(TIMER0_OVF_vect) {
//     constexpr uint8_t val = 0x20;
//     toggle();

//     // if (i < 127)
//     //     PORTB |= val;
//     // else
//     // PORTB &= ~val;
// }
// int main() __attribute__((noreturn));

struct astring {
    char  string[4];
    char *begin = utl::begin(string);
    char *end   = utl::end(string);
};

astring itoa(uint16_t value) {
    auto toHexSingleVal = [](uint8_t in) -> char {
        if (in < 10)
            return '0' + in;
        else
            return 'a' + (in - 10);
    };
    astring  hex;
    uint16_t mask = 0xF000;

    for (auto &character : hex.string) {
        character = toHexSingleVal((value & mask) >> 12);
        value     = value << 4;
    }
    return hex;
}
typedef unsigned int LinkScriptValue;

extern "C" LinkScriptValue __data_start;
extern "C" LinkScriptValue __data_end;
extern "C" LinkScriptValue __bss_start;
extern "C" LinkScriptValue __bss_end;
extern "C" LinkScriptValue __heap_start;
extern "C" LinkScriptValue __heap_end;
extern "C" LinkScriptValue __DATA_REGION_LENGTH__;



extern void *              __brkval;

int main() {
    timer0_init();
    Serial::init();
    static const char myString[] PROGMEM = "Hallo du da\n\r";
    // Serial::tx("123456789 123456789 123456789 123456789 123456789 123456789x\n\r");
    Serial::tx("\n\r\n\r");
    Serial::tx_p(myString);
    Serial::tx(myString);
    Serial::tx("\n\rbss_start: ");
    Serial::tx(itoa(reinterpret_cast<uint16_t>(&__bss_start)).string);
    Serial::tx("\n\rheap_start: ");
    Serial::tx(itoa(reinterpret_cast<uint16_t>(&__heap_start)).string);
    while (Serial::tx_spaceLeft() < 60)
        ;
    Serial::tx("\n\rdata_start: ");
    Serial::tx(itoa(reinterpret_cast<uint16_t>(&__data_start)).string);
    while (Serial::tx_spaceLeft() < 60)
        ;

    Serial::tx("\n\rdata length: ");
    Serial::tx(itoa(reinterpret_cast<uint16_t>(&__DATA_REGION_LENGTH__)).string);
    Serial::tx("\n\r");
    Serial::tx("\n\rbrkval: ");
    Serial::tx(itoa(reinterpret_cast<uint16_t>(__brkval)).string);
    Serial::tx("\n\r");    // while (!Serial::tx("Test the World\n\r"))
    //     ;

    constexpr uint8_t val = 0x20;
    DDRB |= val;

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
