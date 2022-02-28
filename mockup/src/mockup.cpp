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

int main() {
    int *ptr = new int(2);

    print("Hello World\n");
    print("val: \"{:X}\" bin: {:b}, dec: {:d}, {}\n", ptr, int(42), int(42), 2);

    // These will not compile:
    //print("Val: {}"); // static_assert(n_args == sizeof...(Args), "Same number of '{}' needed as arguments!");
    //print("{:x \n", 42); // static_assert(bracket<arr>('{', open + 1) > close);
    //print("{:d}", ptr); // consteval_error("Only {:x} is supported for ptr type");
    delete ptr;

    return 0;
}
