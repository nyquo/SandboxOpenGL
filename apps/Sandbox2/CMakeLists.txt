set(AppName Sandbox2)

set(Headers
    Sandbox.hpp
)

set(Sources
    main.cpp
    Sandbox.cpp
)

# Ressources folder in the sources
set(RessourcesSources ressources)

set(Links
    Core
    Glad
    Renderer
    glm::glm
    imgui
    Utils
)

# Ressources folder for the executable
set(RESSOURCES_FOLDER ${AppName}Ressources)

set(Definitions
    RESSOURCES_FOLDER="${RESSOURCES_FOLDER}"
)

add_executable(${AppName} ${Sources} ${Headers} ${Shaders})
target_link_libraries(${AppName} PRIVATE ${Links})
target_include_directories(${AppName} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(${AppName} PRIVATE ${Definitions})

set_target_properties(${AppName} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>)

# Copy ressources next to executable
get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)


set(TO_COPY "${CMAKE_CURRENT_SOURCE_DIR}/${RessourcesSources}")
set(COPY_TO "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}$<$<BOOL:${GENERATOR_IS_MULTI_CONFIG}>:/$<CONFIG>>/${RESSOURCES_FOLDER}")

set(OUTPUT_AND_DEPENDS "${COPY_TO}/$<JOIN:${Ressources},;${COPY_TO}/>")
add_custom_command(
  OUTPUT "${OUTPUT_AND_DEPENDS}"
  COMMAND ${CMAKE_COMMAND}
    ARGS
      -E copy_directory_if_different
      "${TO_COPY}"
      "${COPY_TO}"
  COMMENT "Copying ressources for ${AppName}"
  COMMAND_EXPAND_LISTS
  DEPENDS ${Ressources}
)
add_custom_target(${AppName}-ressources
  DEPENDS "${OUTPUT_AND_DEPENDS}"
)
add_dependencies(${AppName}
  ${AppName}-ressources
)
