project(utl)
set(utl_headers include/utl/algorithm.h
                include/utl/execution.h
                include/utl/iterator.h
                include/utl/memory.h
)

add_library(utl INTERFACE)

target_include_directories(
    utl
    INTERFACE
        "include"
)
target_compile_features(
    ${PROJECT_NAME}
    INTERFACE
        cxx_std_20
)
