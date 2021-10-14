add_library(gcc-std INTERFACE)
add_library(gcc-std::std ALIAS gcc-std)
target_sources(gcc-std
    INTERFACE
        include/atomic
        include/bit
        include/cfloat
        include/ciso646
        include/climits
        include/compare
        include/concepts
        include/cstdalign
        include/cstdarg
        include/cstdbool
        include/cstddef
        include/cstdint
        include/cstdlib
        include/cxxabi.h
        include/exception
        include/initializer_list
        include/limits
        include/new
        include/typeinfo
        include/type_traits
        include/version
        include/bits/c++config.h
)

target_include_directories(gcc-std
    INTERFACE
        include
)
target_compile_definitions(gcc-std
    INTERFACE
        _GLIBCXX_BEGIN_NAMESPACE_VERSION=
        _GLIBCXX_END_NAMESPACE_VERSION=
        _GLIBCXX17_INLINE=inline
        _GLIBCXX17_DEPRECATED=
)
target_compile_features(gcc-std INTERFACE cxx_std_20)
