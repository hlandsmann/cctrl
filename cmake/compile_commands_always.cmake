include(${CMAKE_SOURCE_DIR}/cmake/get_all_targets.cmake)
# whenever make is executed (in subdirectories, for whatever target) the compile_commands.json link is updated
# This makes sure, that even if the toolchain is changed (switching from x86 to arm, or vise versa), the
# compile_commands.json link will always refer to the last generated one.

# Even though the solution is quite simple, maybe even not very "performant", I could not measure any difference
# with or without this target enabled.

# create symlink for the compile_commands
ADD_CUSTOM_TARGET(compile_commands_json ALL
                  COMMAND ${CMAKE_COMMAND} -E create_symlink
                     ${CMAKE_BINARY_DIR}/compile_commands.json
                     ${CMAKE_SOURCE_DIR}/build/compile_commands.json
                  DEPENDS
                     ${CMAKE_BINARY_DIR}/compile_commands.json
)
get_all_targets(all_targets)
foreach( target ${all_targets} )
    add_dependencies(${target} compile_commands_json)
endforeach()
