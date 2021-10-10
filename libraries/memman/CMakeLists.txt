project(memman)
set(LIBRARY_TARGET_NAME ${PROJECT_NAME})

set(memman_srcs src/memman.cpp)
add_library(memman ${memman_srcs})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})
target_link_libraries(
    memman
    PRIVATE
        ${CMAKE_PROJECT_NAME}_warnings
)
