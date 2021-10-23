project(utl)

add_library(utl INTERFACE)
add_library(utl::utl ALIAS utl)
target_include_directories(utl
    INTERFACE
        "include"
)
target_compile_features(utl
    INTERFACE
        cxx_std_20
)
target_sources(utl
    INTERFACE
        include/utl/algorithm.h
        include/utl/array.h
        include/utl/execution.h
        include/utl/iterator.h
        include/utl/memory.h
)

if (NOT MOCKUP)
target_link_libraries(utl
    INTERFACE
        std-avr::std-avr
)
endif (NOT MOCKUP)
