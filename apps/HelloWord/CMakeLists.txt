set(AppName HelloWord)

file(GLOB_RECURSE Headers
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

file(GLOB_RECURSE Sources
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

file(GLOB_RECURSE Shaders
	${CMAKE_CURRENT_SOURCE_DIR}/*.frag
	${CMAKE_CURRENT_SOURCE_DIR}/*.vert
)

set(Links
    Core
    Glad
)

add_executable(${AppName} ${Sources} ${Headers} ${Shaders})
target_link_libraries(${AppName} PRIVATE ${Links})
target_include_directories(${AppName} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#TODO Use symlink to avoid copy?
#TODO YT comment found on a cmake video: Use a custom target and custom command to run the copying/symlink operation only when necessary. To achieve this, add a custom target that DEPENDS on the desired file/directory in the build directory. Then add a custom command that OUTPUTs the previously set dependency on the custom target. Set the COMMAND you want, and finally, make sure this custom command DEPENDS on the source file you want to copy/symlink. In summary, the custom target depends on the build directory version of our file. How do we satisfy this dependency? With the custom command that outputs that file. When should that custom command run? When its output file does not exist yet, or when the file it depends on (the original resource file) changes.

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(IS_MULTI_CONFIG)
    add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/shaders
        COMMENT "Copying shaders in the output directory"
    )
else()
    add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
        COMMENT "Copying shaders in the output directory"
    )
endif()
add_dependencies(${AppName} copy_shaders)
