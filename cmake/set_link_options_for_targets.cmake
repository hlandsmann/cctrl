include(${CMAKE_SOURCE_DIR}/cmake/get_all_targets.cmake)
get_all_targets(all_targets)
get_property(link_level GLOBAL PROPERTY GLOBAL_LINK_OPTIONS)
foreach( target ${all_targets} )
    get_target_property(target_type ${target} TYPE)
    if ( target_type STREQUAL "EXECUTABLE" OR
         target_type STREQUAL "STATIC_LIBRARY" OR
         target_type STREQUAL "SHARED_LIBRARY")
        message(STATUS "Applying link options for ${target_type} ${target} as per toolchain")
        target_link_options(${target} PRIVATE ${link_level})
    endif ()
endforeach()
