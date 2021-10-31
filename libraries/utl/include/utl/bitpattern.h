#pragma once

// This header was 'borrowed' from <<C++ weekly with json turner>> and adapted somewhat to fit my needs

#include <cstdint>
#include <type_traits>

namespace utl {

class BitPattern {
private:
    // expected is the expected result after masking
    std::uint16_t expected{};

    // mask is set of bits I care about
    std::uint16_t mask{0xFFFF};

    // input & mask == expected

public:
    template <std::size_t Size> explicit consteval BitPattern(const char (&input)[Size]) {
        std::uint16_t cur_bit = 1;
        cur_bit <<= (Size - 2);

        for (const char val : input) {
            if (val == 0) {
                return;
            }

            if (val == '1') {
                expected |= cur_bit;
            } else if (val == '0') {
            } else if (val == '-') {
                // 000010 // cur_bit
                // 111101 // bitwise complement

                // 110111 // hypothetical current mask
                //&111101 // bitwise and
                //=110101 // new mask, with cur_bit cleared from it
                mask &= ~cur_bit;
            } else {
                using error_message = const char[];
                new error_message("Unknown characters in BitPattern input");
            }

            cur_bit >>= 1;
        }
    }

    template <typename ValueType>
    constexpr friend bool operator==(const ValueType value,
                                     const BitPattern &pattern) requires(std::is_unsigned_v<ValueType>) {
        return (value & pattern.mask) == pattern.expected;
    }
};
}  // namespace utl
