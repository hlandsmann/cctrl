#pragma once
#include <stddef.h>
#include <stdint.h>

namespace utl {
using size8_t = uint8_t;

template <class T, size_t N> constexpr size_t size(const T (&)[N]) noexcept { return N; }
template <class T, size_t N> constexpr T* begin(T (&array)[N]) noexcept { return array; }
template <class T, size_t N> constexpr T* end(T (&array)[N]) noexcept { return array + N; }

template <typename RandomAccessIterator>
inline constexpr auto distance(RandomAccessIterator first, RandomAccessIterator last) {
    return last - first;
}

}  // namespace utils
