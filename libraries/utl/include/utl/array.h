#pragma once
#include <array>
#include <stddef.h>

namespace utl {
template <class T, size_t N> struct array : std::array<T, N> {};

template <class T, size_t N> struct array_progmem : std::array<T, N> {
    constexpr array_progmem(const std::array<T, N> & arr) : std::array<T, N>(arr) {};
};
}  // namespace utl
