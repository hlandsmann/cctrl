#pragma once
#include <string.h>
#include <avr/pgmspace.h>
namespace utl {
template <class InputIt, class OutputIt> auto copy(InputIt first, InputIt last, OutputIt d_first) {
    while (first != last) {
        *d_first++ = *first++;
    }
    return d_first;
}

template <class InputIt, class OutputIt> auto copy_p(InputIt first, InputIt last, OutputIt d_first) {
    while (first != last) {
        if constexpr (sizeof(*first) == 1)
            *d_first++ = pgm_read_byte(first++);
        else
            static_assert(sizeof(*first) == 1);
    }
    return d_first;
}

template <class InputPtr, class T_size, class OutputPtr>
OutputPtr memcopy(InputPtr first, T_size size, OutputPtr d_first) {
    static_assert(sizeof(*first) == sizeof(*d_first));
    return reinterpret_cast<OutputPtr>(memcpy((void*)d_first, (void*)first, sizeof(*first) * size));
}
template <class T> const T& min(const T& a, const T& b) { return (a < b) ? a : b; }
template <class T> const T& max(const T& a, const T& b) { return (a > b) ? a : b; }
}  // namespace utl
