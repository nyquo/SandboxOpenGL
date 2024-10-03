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
    glm::glm
)

set(RESSOURCES_FOLDER ${AppName}Ressources)

set(Definitions
    RESSOURCES_FOLDER="${RESSOURCES_FOLDER}"
)

add_executable(${AppName} ${Sources} ${Headers} ${Shaders})
target_link_libraries(${AppName} PRIVATE ${Links})
target_include_directories(${AppName} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(${AppName} PRIVATE ${Definitions})

#TODO Use symlink to avoid copy?
#TODO YT comment found on a cmake video: Use a custom target and custom command to run the copying/symlink operation only when necessary.
#To achieve this, add a custom target that DEPENDS on the desired file/directory in the build directory. Then add a custom command that OUTPUTs the previously set dependency on the custom target.
# Set the COMMAND you want, and finally, make sure this custom command DEPENDS on the source file you want to copy/symlink. In summary, the custom target depends on the build directory version of our file.
#How do we satisfy this dependency? With the custom command that outputs that file. When should that custom command run? When its output file does not exist yet, or when the file it depends on (the original resource file) changes.

# TODO replace by copy_directory_if_different with cmake 3.26

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(IS_MULTI_CONFIG)
    add_custom_target(copy_shaders${AppName} ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${RESSOURCES_FOLDER}/shaders
        COMMENT "Copying ${AppName} shaders in the output directory"
    )
    add_custom_target(copy_assets${AppName} ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${RESSOURCES_FOLDER}/assets
        COMMENT "Copying ${AppName} assets in the output directory"
    )
else()
    add_custom_target(copy_shaders${AppName} ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${RESSOURCES_FOLDER}/shaders
        COMMENT "Copying ${AppName} shaders in the output directory"
    )
    add_custom_target(copy_assets${AppName} ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${RESSOURCES_FOLDER}/assets
        COMMENT "Copying ${AppName} assets in the output directory"
    )
endif()
add_dependencies(${AppName} copy_shadersHelloWord)
add_dependencies(${AppName} copy_assetsHelloWord)

set_target_properties(${AppName} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>)
