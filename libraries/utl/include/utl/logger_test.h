#pragma once

#include <serial/serial.h>
#include <utl/array.h>

#include <algorithm>
#include <bit>
#include <concepts>
#include <functional>
#include <limits>
#include <span>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace utl {

namespace logger {

enum class Base : uint8_t { bin = 2, oct = 8, dec = 10, hex = 16 };

template <typename T>
consteval auto getItoaStringLength(Base base) -> size_t requires(std::is_integral_v<T>) {
    T startValue = std::numeric_limits<T>::max();
    size_t result = 1;
    while (0 != (startValue /= static_cast<uint8_t>(base))) {
        result++;
    }
    if (base == Base::dec && std::is_signed_v<T>)
        result++;
    return result;
}

template <Base base> auto singleVal(uint8_t in) -> char requires(base == Base::hex) {
    if (in < 10)
        return '0' + in;
    else
        return 'a' + (in - 10);
}
template <Base base> auto singleVal(uint8_t in) -> char requires(base != Base::hex) { return '0' + in; }

template <typename Integral, Base base>
void itoa(Integral value, const std::span<char>& outp) requires(
    std::is_integral_v<Integral>&& std::is_unsigned_v<Integral> && sizeof(Integral) >= sizeof(int)) {
    std::generate(outp.rbegin(), outp.rend(), [startVal = value]() mutable -> char {
        constexpr uint8_t div = static_cast<uint8_t>(base);
        if (startVal == 0) {
            return 0;
        }
        uint8_t intermediate = startVal % div;
        startVal /= div;
        return singleVal<base>(intermediate);
    });
}

template <typename Integral, Base base>
void itoa(Integral value, const std::span<char>& outp) requires(
    std::is_integral_v<Integral>&& std::is_integral_v<Integral>&& std::is_signed_v<Integral>&& base ==
        Base::dec &&
    sizeof(Integral) >= sizeof(int)) {
    using CByte = typename std::conditional_t<std::is_signed_v<Integral>, int8_t, uint8_t>;

    bool put_minus = (value < 0);

    std::generate(outp.rbegin(), outp.rend(), [startVal = value, &put_minus]() mutable -> char {
        constexpr CByte div = static_cast<CByte>(base);

        if (startVal == 0 && put_minus) {
            put_minus = false;
            return '-';
        }
        if (startVal == 0) {
            return 0;
        }
        char nextCharacter;
        CByte intermediate = startVal % div;
        startVal /= div;
        if (intermediate < 0)
            intermediate *= -1;

        nextCharacter = singleVal<base>(intermediate);

        return nextCharacter;
    });
}

template <typename Integral, Base base>
void itoa(Integral value, const std::span<char>& outp) requires(
    std::is_integral_v<Integral>&& std::is_signed_v<Integral>&& base != Base::dec &&
    sizeof(Integral) >= sizeof(int)) {
    using UIntegral = typename std::make_unsigned<Integral>::type;
    itoa<UIntegral, Base::hex>(std::bit_cast<UIntegral>(value), outp);
}

template <typename Integral, Base base>
void itoa(Integral value, const std::span<char>& outp) requires(std::is_integral_v<Integral> &&
                                                                sizeof(Integral) < sizeof(int)) {
    using Integer =
        typename std::conditional_t<std::is_signed_v<Integral>, int, std::make_unsigned<int>::type>;
    itoa<Integer, Base::hex>(value, outp);
}

template <typename Ptr, Base base>
void itoa(Ptr value, const std::span<char>& outp) requires(std::is_pointer_v<Ptr>) {
    itoa<std::uintptr_t, base>(std::bit_cast<std::uintptr_t>(value, outp));
}

template <class ValueType, auto... modi> void printSingleValue(ValueType value) {
    constexpr auto modi_tuple = std::make_tuple(modi...);
    using print_type = std::decay_t<ValueType>;
    const auto outputString = [&]() {
        if constexpr (std::is_integral_v<print_type> || std::is_pointer_v<print_type>) {
            std::array<char, getItoaStringLength<print_type>(std::get<0>(modi_tuple))> result;
            itoa<print_type, std::get<0>(modi_tuple)>(value, std::span(result));
            return result;
        }
    }();
    Serial::tx(outputString);
}
template <size_t N> consteval auto cleanOutpString(const char (&str)[N]) -> std::array<char, N - 1> {
    std::array<char, N - 1> result;
    std::copy(std::begin(str), std::end(str) - 1, std::begin(result));
    return {result};
}
template <size_t N> consteval auto rawFormatString(const char (&str)[N]) -> std::array<char, N - 1> {
    std::array<char, N - 1> result;
    std::copy(std::begin(str), std::end(str) - 1, std::begin(result));

    return {result};
}
template <std::array rawStr, size_t N, typename... Args>
void t(const std::array<char, N>& text, Args... args) {
    constexpr auto seq = std::make_index_sequence<sizeof...(Args)>();
    const auto arg_tuple = std::make_tuple(args...);

    Serial::tx_p(text);

    [&]<size_t... I>(std::index_sequence<I...>) {
        (printSingleValue<decltype(std::get<I>(arg_tuple)), Base::dec>(std::get<I>(arg_tuple)), ...);
    }
    (seq);
}
}  // namespace logger
}  // namespace utl
#define print(format, ...)                                                   \
    [&] {                                                                    \
        namespace logger = utl::logger;                                      \
        static const auto log_str PROGMEM = logger::cleanOutpString(format); \
        logger::t<logger::rawFormatString(format)>(log_str, __VA_ARGS__);    \
    }()
