add_executable(mockup)
target_sources(mockup
    PRIVATE
        src/mockup.cpp
)
target_compile_features(mockup PRIVATE cxx_std_20)
target_link_libraries(mockup
    PRIVATE
        utl::utl
        serial::serial
)
