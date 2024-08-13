set(AppName HelloWord)

set(Sources
    main.cpp
    shaders/BasicShader.frag
    shaders/BasicShader.vert
    HelloWordApp.cpp
)

set(Headers HelloWordApp.hpp)

set(Links
    Core
    Glad
)

add_executable(${AppName} ${Sources} ${Headers})
target_link_libraries(${AppName} PRIVATE ${Links})
target_include_directories(${AppName} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(IS_MULTI_CONFIG)
    foreach(_build_type ${CMAKE_CONFIGURATION_TYPES})
        add_custom_command(
            TARGET ${AppName} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
                    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_build_type}/shaders)
    endforeach()
else()
    add_custom_command(
        TARGET ${AppName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/shaders
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
endif()
