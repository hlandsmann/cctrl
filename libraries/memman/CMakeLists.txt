project(memman)

add_library(memman STATIC)
target_sources(memman PRIVATE src/memman.cpp)
add_library(memman::memman ALIAS memman)

target_include_directories(
    memman
    PRIVATE
        include/memman
    INTERFACE
        include
)

target_link_libraries(
    memman
    PRIVATE
        ${CMAKE_PROJECT_NAME}_warnings
        umm_malloc::umm_malloc
        utl::utl
)
