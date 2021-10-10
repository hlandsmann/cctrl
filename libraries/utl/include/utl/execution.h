#pragma once

#include <avr/interrupt.h>
namespace utl {
struct ClearInterrupts {
    ClearInterrupts() { cli(); };
    ~ClearInterrupts() { sei(); };
};
}  // namespace utils
