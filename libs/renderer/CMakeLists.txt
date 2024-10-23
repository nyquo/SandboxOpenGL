set(LibName Renderer)

file(GLOB_RECURSE Headers
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

file(GLOB_RECURSE Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

set(PrivateLinks
    Glad
    Core
)

set(PublicLinks
    Io
    glm::glm
    assimp::assimp
)

set(RESSOURCES_FOLDER ${LibName}Ressources)

set(Definitions
    RESSOURCES_FOLDER="${RESSOURCES_FOLDER}"
)

# For some reason building core shared produced runtime error with glad on windows
# As fighting we cmake and windows is not the purpose of this repo, this will be build
# statically on windows for now
if(WIN32)
        add_library(${LibName} STATIC
                ${Headers}
                ${Sources}
        )

#	if(BUILD_SHARED_LIBS)
#		target_compile_definitions(${LibName} PRIVATE RENDERER_API_EXPORT)
#	endif()
else()
        add_library(${LibName}
                ${Headers}
                ${Sources}
        )
endif()



target_link_libraries(${LibName} PRIVATE ${PrivateLinks})
target_link_libraries(${LibName} PUBLIC ${PublicLinks} )
target_include_directories(${LibName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(${LibName} PRIVATE ${Definitions})

#TODO Use symlink to avoid copy?
#TODO YT comment found on a cmake video: Use a custom target and custom command to run the copying/symlink operation only when necessary.
#To achieve this, add a custom target that DEPENDS on the desired file/directory in the build directory. Then add a custom command that OUTPUTs the previously set dependency on the custom target.
# Set the COMMAND you want, and finally, make sure this custom command DEPENDS on the source file you want to copy/symlink. In summary, the custom target depends on the build directory version of our file.
#How do we satisfy this dependency? With the custom command that outputs that file. When should that custom command run? When its output file does not exist yet, or when the file it depends on (the original resource file) changes.

# TODO replace by copy_directory_if_different with cmake 3.26

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(IS_MULTI_CONFIG)
    add_custom_target(copy_shaders${LibName} ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${RESSOURCES_FOLDER}/shaders
        COMMENT "Copying ${LibName} shaders in the output directory"
    )
else()
    add_custom_target(copy_shaders${LibName} ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${RESSOURCES_FOLDER}/shaders
        COMMENT "Copying ${LibName} shaders in the output directory"
    )
endif()
add_dependencies(${LibName} copy_shaders${LibName})
