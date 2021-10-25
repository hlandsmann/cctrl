#define PROGMEM

#include <utl/logger.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <iostream>
#include <ranges>
#include <string_view>

namespace ranges = std::ranges;

// void test() {
//     constexpr auto mystr = array_from_str("val: \"{}\" hex: {:x}, dec: {:d}\n");
//     std::string_view sv{mystr.begin(), mystr.end()};
//     std::cout << sv << " - nargs: " << count_args<mystr>() << "\n";
//     constexpr auto args = get_args<mystr>();

//     std::cout << "Tuple size: " << std::tuple_size_v<decltype(args)> << "\n";

//     constexpr auto no_fmt = remove_fmt<mystr>();
//     std::string_view no_fmt_sv{no_fmt.begin(), no_fmt.end()};
//     std::cout << "No FMT: " << no_fmt_sv;

//     std::array<char, 5> testa;
// }

int main() {
    int *ptr = new int(2);
    int myvalue = 42;
    print("val: \"{}\" bin: {:b}, dec: {:d}, {}\n", ptr, int(42), int(42), 2);
    print("Hello world: \n");
    delete ptr;
    // print("hello world", uint(8), int(8));
    // print("\n");

    // const char* ptr = "abc";
    // print("pointer: ", ptr);
    // print("\n\n");

    // using Base = utl::logger::Base;
    // // Serial::tx(utl::logger::itoa<uint16_t, Base::hex>(reinterpret_cast<uint16_t>(ptr)));
    // uint8_t test = 10;
    // utl::logger::printSingleValue<uint8_t, Base::dec>(test);
    // print("\n");
    // utl::logger::printSingleValue<uint8_t, Base::hex>(11);
    // print("\n");
    // utl::logger::printSingleValue<int8_t, Base::hex>(12);
    // print("\n");
    // utl::logger::printSingleValue<int16_t, Base::dec>(12);
    // print("\n");
    // utl::logger::printSingleValue<uint16_t, Base::dec>(13);
    // print("\n");

    // test();
    return 0;
}
