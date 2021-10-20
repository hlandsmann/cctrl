#pragma once

#include <serial/serial.h>
#include <utl/array.h>
#include <algorithm>
#include <bit>
#include <concepts>
#include <functional>
#include <limits>
#include <string_view>

#include <tuple>
#include <type_traits>
#include <utility>

namespace utl {

namespace logger {

enum class Base : uint8_t { bin = 2, oct = 8, dec = 10, hex = 16 };

template <size_t N> consteval void  consteval_err(const char (&str)[N]) {
    if (N)
        new (const char*)(str);
}

template <typename T>
consteval auto getStringLength(Base base) -> size_t requires(std::is_integral_v<T>) {
    T startValue = std::numeric_limits<T>::max();
    size_t result = 1;
    while (0 < (startValue /= static_cast<uint8_t>(base))) {
        result++;
    }
    return result;
}

template <typename Integer, Base base>
auto itoa(Integer value) -> std::array<char, getStringLength<Integer>(base)>
requires(std::is_integral_v<Integer>) {
    auto singleVal = [](uint8_t in) -> char {
        if (in < 10)
            return '0' + in;
        else
            return 'a' + (in - 10);
    };
    std::array<char, getStringLength<Integer>(base)> result;
    std::generate(result.rbegin(), result.rend(), [startVal = value, &singleVal]() mutable -> char {
        constexpr uint8_t div = static_cast<uint8_t>(base);
        if (startVal == 0)
            return 0;
        char nextCharacter = singleVal(startVal % div);
        startVal /= div;
        return nextCharacter;
    });

    return result;
}

template <typename Ptr, Base base>
auto itoa(Ptr value) -> std::array<char, getStringLength<std::uintptr_t>(base)>
requires(std::is_pointer_v<Ptr>) {
    return itoa<std::uintptr_t, base>(std::bit_cast<std::uintptr_t>(value));
}

template <typename... Args>
auto converse(Args&&... args) -> std::tuple<std::array<char, getStringLength<Args>(Base::dec)>...> {
    return std::make_tuple(itoa<Args, Base::dec>(args)...);
}

template <class ValueType, auto... modi> void printSingleValue(ValueType value) {
    constexpr auto modi_tuple = std::make_tuple(modi...);
    using print_type = std::decay_t<ValueType>;
    const auto outputString = [&]() {
        if constexpr (std::is_integral_v<print_type> || std::is_pointer_v<print_type>)
            return itoa<print_type, std::get<0>(modi_tuple)>(value);
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
    if(std::size(str) >1000) consteval_err("size is limited");
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

/*
#define print(format, ...)                                                                 \
    [&]() {                                                                                \
        namespace logger = utl::logger;                                                    \
        using Base = utl::logger::Base;                                                    \
        const auto arg_tuple = std::make_tuple(__VA_ARGS__);                               \
        constexpr std::string_view format_str = format;                                    \
        auto fff = logger::t<logger::rawFormatString(format)>(__VA_ARGS__);                \
        std::apply(                                                                        \
            [](auto... value) {                                                            \
                ((utl::logger::printSingleValue<decltype(value), Base::dec>(value)), ...); \
            },                                                                             \
            arg_tuple);                                                                    \
    }()
 */
#define print(format, ...)                                                   \
    [&] {                                                                    \
        namespace logger = utl::logger;                                      \
        static const auto log_str PROGMEM = logger::cleanOutpString(format); \
        logger::t<logger::rawFormatString(format)>(log_str, __VA_ARGS__);    \
    }()
