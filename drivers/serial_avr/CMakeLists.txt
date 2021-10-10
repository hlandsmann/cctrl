project(serial)

set(serial_srcs src/serial.cpp)
set(serial_hdrs include/serial/serial.h)
add_library(serial ${serial_srcs} ${serial_hdrs})
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
