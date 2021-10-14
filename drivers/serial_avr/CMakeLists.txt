project(serial)

add_library(serial STATIC)
target_sources(serial
    PRIVATE
        src/serial.cpp
    PUBLIC
        include/serial/serial.h
)

target_include_directories(
    serial
    PRIVATE
        include/serial
        src
    INTERFACE
        include
)
target_link_libraries(
    serial
    PRIVATE
        ${CMAKE_PROJECT_NAME}_warnings
        utl
)
