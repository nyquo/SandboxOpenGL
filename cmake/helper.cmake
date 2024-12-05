# Create a new executable the SandboxOpenGL project (let's call it sogl)
#
# Usage:
#   sogl_add_executable(exe_name
#                SOURCES [item1...]
#                [INCLUDES [item1...] ]
#                [LINKS [item1...] ]
#                [DEFINITIONS [item1 ...]]
#                [RESSOURCES [item1 ...]]
#
# SOURCES are the sources of the executable
# INCLUDES an optional list of include directories that are dependencies
# LINKS an optional list of targets/libraries to link with as dependencies
# DEFINITIONS an optional list of definitions to add to the target
# RESSOURCES an optinal list of ressources to copy next to the executable
#
function(sogl_add_executable AppName)
    set(_options "")
    set(_singleValues)
    set(_multipleValues SOURCES INCLUDES LINKS DEFINITIONS RESSOURCES)

    cmake_parse_arguments(PARAM "${_options}" "${_singleValues}" "${_multipleValues}" ${ARGN})

    if(NOT AppName)
        message(FATAL_ERROR "You must provide the executable name in 'sogl_add_executable'")
    endif()

    if(NOT PARAM_SOURCES)
        message(FATAL_ERROR "You must provide the executable SOURCES in 'sogl_add_executable'")
    endif()

    add_executable(${AppName} ${PARAM_SOURCES} ${PARAM_RESSOURCES})

    target_link_libraries(${AppName} PRIVATE ${PARAM_LINKS})
    target_include_directories(${AppName} PRIVATE ${PARAM_INCLUDES})

    if(PARAM_DEFINITIONS)
        target_compile_definitions(${AppName} PRIVATE ${PARAM_DEFINITIONS})
    endif()

    set_target_properties(${AppName} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>)

    # Inspired from https://discourse.cmake.org/t/copy-resources-to-build-folder/1010/5
    # Copy ressources next to executable
    if(PARAM_RESSOURCES)
        get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

        set(RESSOURCES_FOLDER ${AppName}Ressources)
        target_compile_definitions(${AppName} PRIVATE RESSOURCES_FOLDER="${RESSOURCES_FOLDER}")
        set(COPY_TO "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}$<$<BOOL:${GENERATOR_IS_MULTI_CONFIG}>:/$<CONFIG>>/${RESSOURCES_FOLDER}")

        foreach(RESSOURCE ${PARAM_RESSOURCES})
            set(DEST_FILE "${COPY_TO}/${RESSOURCE}")

            add_custom_command(
                OUTPUT "${DEST_FILE}"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${CMAKE_CURRENT_SOURCE_DIR}/${RESSOURCE}" "${DEST_FILE}"
                COMMENT "Copying resource: ${RESSOURCE} for ${AppName}"
                DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${RESSOURCE}"
            )
            list(APPEND RESOURCE_OUTPUTS "${DEST_FILE}")
        endforeach()

        add_custom_target(${AppName}-ressources
            DEPENDS ${RESOURCE_OUTPUTS}
        )
        add_dependencies(${AppName} ${AppName}-ressources)
    endif()

endfunction()
