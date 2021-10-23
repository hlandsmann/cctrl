include(${CMAKE_SOURCE_DIR}/cmake/get_all_targets.cmake)

function(set_link_options_for_all_targets)
    get_all_targets(all_targets)
    foreach( target ${all_targets} )
        get_target_property(target_type ${target} TYPE)
        if ( target_type STREQUAL "EXECUTABLE" OR
            target_type STREQUAL "STATIC_LIBRARY")
            message(STATUS "Applying link options for ${target_type} ${target} as per toolchain..")
            get_property(link_options GLOBAL PROPERTY GLOBAL_LINK_OPTIONS)
            target_link_options(${target} PRIVATE ${link_options})
        endif ()
    endforeach()
endfunction(set_link_options_for_all_targets link_options)
