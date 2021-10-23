project(printf)

add_library(printf OBJECT)
add_library(printf::printf ALIAS printf)
target_sources(printf
    PRIVATE
        src/printf.c
    PUBLIC
        src/printf.h
)

target_include_directories(printf
    PUBLIC
        src
)

target_link_libraries(printf
    PRIVATE
        serial::serial
)
