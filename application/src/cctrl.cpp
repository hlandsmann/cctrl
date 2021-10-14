#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include <memman/memman.h>
#include <serial/serial.h>
#include <utl/execution.h>


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
    TCCR0A = TCCR0A | (1 << WGM01);
    // Set CS01 and CS00 bits for 64 prescaler
    TCCR0B = TCCR0B | pre1024;
    // TCCR0B |= (1 << CS01);

    // enable timer compare interrupt
    TIMSK0 = TIMSK0 | (1 << OCIE0A);
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

    PORTB = PORTB ^ val;
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
    char string[4];
    char *begin = utl::begin(string);
    char *end = utl::end(string);
};

astring itoa(uint16_t value) {
    auto toHexSingleVal = [](uint8_t in) -> char {
        if (in < 10)
            return '0' + in;
        else
            return 'a' + (in - 10);
    };
    astring hex;
    uint16_t mask = 0xF000;

    for (auto &character : hex.string) {
        character = toHexSingleVal((value & mask) >> 12);
        value = value << 4;
    }
    return hex;
}

extern void *__brkval;
// # define F(s) (__extension__({static const char __c[15] PROGMEM = (s); reinterpret_cast<const
// char[15]>( __c);}))

template <size_t N> constexpr auto F(const char (&str)[N]) -> const char * { return str; }

template <size_t N> struct str_test {
    explicit consteval str_test(const char (&str)[N]) {
        for (size_t i = 0; i < N - 1; i++)
            string[i] = str[i];
    };
    char string[N - 1];
};

#define log(str)                                               \
    []() {                                                     \
        static const str_test log_str PROGMEM = str_test(str); \
        Serial::tx_p(log_str.string);                          \
    }()

int main() {
    timer0_init();
    Serial::init();

    memman::initMemory();

    log("\n\r\n\r");
    while (Serial::tx_spaceLeft() < 60)
        ;
    log("Hello World\n\r");

    for (int i = 0; i < 256; i++) {
        while (Serial::tx_spaceLeft() < 60)
            ;
        log("\n\rPtr: ");
        uint16_t *ptr = new uint16_t[10];
        if (ptr != nullptr)
            Serial::tx(itoa(reinterpret_cast<uint16_t>(ptr)).string);
        else
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
