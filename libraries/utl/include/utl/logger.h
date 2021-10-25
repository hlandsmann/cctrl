#pragma once

#include <serial/serial.h>
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

template <typename T> struct always_false : std::false_type {};

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
template <Base base> auto singleVal(uint8_t in) -> char requires(base != Base::hex) {
    return '0' + in;
}

template <std::unsigned_integral Integral, Base base>
auto itoa(Integral value) -> std::array<char, getItoaStringLength<Integral>(base)> {
    std::array<char, getItoaStringLength<Integral>(base)> result;
    std::generate(result.rbegin(), result.rend(), [startVal = value]() mutable -> char {
        constexpr uint8_t div = static_cast<uint8_t>(base);
        if (startVal == 0) {
            return 0;
        }
        uint8_t intermediate = startVal % div;
        startVal /= div;
        return singleVal<base>(intermediate);
    });

    return result;
}

template <typename Integral, Base base>
auto itoa(Integral value) -> std::array<char, getItoaStringLength<Integral>(base)>
requires(std::is_integral_v<Integral>&& std::is_signed_v<Integral>&& base == Base::dec) {
    std::array<char, getItoaStringLength<Integral>(base)> result;
    using CByte = typename std::conditional_t<std::is_signed_v<Integral>, int8_t, uint8_t>;

    bool put_minus = (value < 0);

    std::generate(result.rbegin(), result.rend(), [startVal = value, &put_minus]() mutable -> char {
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

    return result;
}

template <std::signed_integral Integral, Base base>
auto itoa(Integral value) -> std::array<char, getItoaStringLength<std::make_unsigned_t<Integral>>(base)>
requires(base != Base::dec) {
    using UIntegral = typename std::make_unsigned<Integral>::type;
    return itoa<UIntegral, base>(std::bit_cast<UIntegral>(value));
}

template <class T>
concept pointer = std::is_pointer_v<T>;

template <pointer Ptr, Base base>
auto itoa(Ptr value) -> std::array<char, getItoaStringLength<std::uintptr_t>(Base::hex)> {
    return itoa<std::uintptr_t, Base::hex>(std::bit_cast<std::uintptr_t>(value));
}

template <std::array arr> consteval auto bracket(char br, size_t begin = 0) -> size_t {
    auto it = std::find(arr.begin() + begin, arr.end(), br);
    if (it == arr.end())
        return arr.size();
    return std::distance(arr.begin(), it);
}

template <std::array arr, size_t begin = 0> consteval auto count_args(size_t count = 0) -> size_t {
    constexpr size_t open = bracket<arr>('{', begin);
    constexpr size_t close = bracket<arr>('}', begin);
    if constexpr (open == arr.size() && close == arr.size())
        return count;
    else {
        static_assert(open < close);
        static_assert(close != arr.size());
        static_assert(bracket<arr>('{', open + 1) > close);
    }
    return count_args<arr, close + 1>(count + 1);
}

template <auto val> struct value_type_of {
private:
    using U = typename std::decay_t<decltype(val)>;

public:
    using value_type = U::value_type;
};
template <auto val> using value_type_of_t = value_type_of<val>::value_type;

template <std::array arr, size_t first, size_t last>
consteval auto subarray() -> std::array<value_type_of_t<arr>, last - first> {
    static_assert(first <= last);
    std::array<value_type_of_t<arr>, last - first> result;
    std::copy(arr.begin() + first, arr.begin() + last, result.begin());
    return result;
}

template <std::array arr, size_t begin = 0, size_t fmt_overhead = 0> consteval auto get_args() {
    constexpr size_t open = bracket<arr>('{', begin);
    constexpr size_t close = bracket<arr>('}', begin);

    if constexpr (open == arr.size())
        return std::tuple<>();
    else {
        constexpr auto cmd = subarray<arr, open + 1, close>();
        constexpr auto prepend_tuple = std::make_tuple(open - fmt_overhead, cmd);
        return std::tuple_cat(std::make_tuple(prepend_tuple),
                              get_args<arr, close + 1, 1 + fmt_overhead + (close - open)>());
    }
}

template <std::array array1, std::array array2> consteval auto concat() {
    using type = value_type_of_t<array1>;
    constexpr size_t concat_size = array1.size() + array2.size();
    std::array<type, concat_size> result;
    auto outIt = result.begin();
    outIt = std::copy(array1.begin(), array1.end(), outIt);
    std::copy(array2.begin(), array2.end(), outIt);
    return result;
}

template <std::array arr> consteval auto remove_fmt() {
    constexpr size_t open = bracket<arr>('{');
    constexpr size_t close = bracket<arr>('}');

    if constexpr (open == arr.size())
        return arr;
    else {
        constexpr auto result_array = subarray<arr, 0, open>();
        constexpr auto forward_array = subarray<arr, close + 1, arr.size()>();
        return concat<result_array, remove_fmt<forward_array>()>();
    }
}

template <size_t N> consteval auto array_from_str(const char (&str)[N]) -> std::array<char, N - 1> {
    std::array<char, N - 1> result;
    std::copy(std::begin(str), std::end(str) - 1, result.begin());
    return result;
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

template <size_t N> void consteval_error(const char (&str)[N]);

template <typename T> struct fmt_modi_for : std::false_type {};
template <std::integral Int> struct fmt_modi_for<Int> : std::true_type {
    consteval auto operator()() {
        return std::make_tuple(Base::dec);
    }
    template <size_t N> consteval auto operator()(std::array<char, N> fmt) {
        std::string_view fmt_sv{fmt.begin(), fmt.end()};
        if (fmt_sv == ":b")
            return std::make_tuple(Base::bin);
        else if (fmt_sv == ":d")
            return std::make_tuple(Base::dec);
        else if (fmt_sv == ":o")
            return std::make_tuple(Base::oct);
        else if (fmt_sv == ":x")
            return std::make_tuple(Base::hex);
        else
            consteval_error("Only {:b}, {:d}, {:o}, {:x} is supported for Integral type");
    }
};

template <pointer Ptr> struct fmt_modi_for<Ptr> : std::true_type {
    consteval auto operator()() {
        return std::make_tuple(Base::hex);
    }
    template <size_t N> consteval auto operator()(std::array<char, N> fmt) {
        std::string_view fmt_sv{fmt.begin(), fmt.end()};
        if (fmt_sv == ":x")
            return std::make_tuple(Base::hex);
        else
            consteval_error("Only {:x} is supported for ptr type");
    }
};

template <std::array cmd, typename T> consteval auto parse_cmd() {
    if constexpr (cmd.empty()) {
        if constexpr (fmt_modi_for<T>::value) {
            return fmt_modi_for<T>()();
        } else {
            return std::make_tuple();
        }
    } else {
        if constexpr (fmt_modi_for<T>::value) {
            return fmt_modi_for<T>()(cmd);
        } else {
            static_assert(always_false<T>::value, "no format command is known for type");
            return std::make_tuple();
        }
    }
}

template <std::array rawStr, size_t N, typename... Args>
void t(const std::array<char, N>& text, Args... args) {
    constexpr auto seq = std::make_index_sequence<sizeof...(Args)>();
    constexpr auto fmt_tuple = get_args<rawStr>();
    const auto arg_tuple = std::make_tuple(args...);

    uint8_t print_text_from = 0;
    [&]<size_t... I>(std::index_sequence<I...>) {
        (
            [&]() {
                constexpr auto fmt_now = std::get<I>(fmt_tuple);
                constexpr size_t print_text_until = std::get<0>(fmt_now);
                constexpr auto cmd = std::get<1>(fmt_now);
                using Type = typename std::decay_t<decltype(std::get<I>(std::make_tuple(args...)))>;
                constexpr auto modi = parse_cmd<cmd, std::decay_t<decltype(std::get<I>(arg_tuple))>>();
                constexpr auto modi_seq = std::make_index_sequence<std::tuple_size_v<decltype(modi)>>();

                static_assert(N >= print_text_until);
                if constexpr (print_text_until != 0 ) {
                    Serial::tx(text.begin() + print_text_from, text.begin() + print_text_until, true);
                    print_text_from = print_text_until;
                }
                [&]<size_t... J>(std::index_sequence<J...>) {
                    printSingleValue<Type, (std::get<J>(modi), ...)>(std::get<I>(arg_tuple));
                }
                (modi_seq);
            }(),
            ...);
    }
    (seq);
    if constexpr (sizeof...(Args) != 0) {
        constexpr size_t last_chunk_begin = std::get<0>(std::get<sizeof...(Args) - 1>(fmt_tuple));
        if constexpr (last_chunk_begin < N)
            Serial::tx(text.begin() + last_chunk_begin, text.end(), true);
    } else
        Serial::tx_p(text);
}

template <std::array format, typename... Args> void test_validity(Args...) {
    static_assert(count_args<format>() == sizeof...(Args));
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
#define VA_ARGS(...) , ##__VA_ARGS__
#define print(format, ...)                                                      \
    [&] {                                                                       \
        namespace logger = utl::logger;                                         \
        constexpr auto format_array = logger::array_from_str(format);           \
        logger::test_validity<format_array>(__VA_ARGS__);                       \
                                                                                \
        static const auto log_str PROGMEM = logger::remove_fmt<format_array>(); \
        logger::t<format_array>(log_str VA_ARGS(__VA_ARGS__));                  \
    }()
