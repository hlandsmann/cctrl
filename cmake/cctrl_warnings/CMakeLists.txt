set(TARGET ${CMAKE_PROJECT_NAME}_warnings)

add_library(${TARGET} INTERFACE)

target_compile_options(
    ${TARGET}
    INTERFACE
        -Wall
        -Wextra
        -pedantic
)
