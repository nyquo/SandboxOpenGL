set(AppName Sandbox2)

set(HEADERS
    Sandbox.hpp
)

set(SOURCES
    main.cpp
    Sandbox.cpp
)

set(RESSOURCES
    shaders/LightCubeShader.frag
    shaders/LightCubeShader.vert
)

set(LINKS
    Core
    Glad
    Renderer
    glm::glm
    imgui
    Utils
)
set(DEFIITIONS
)

sogl_add_executable(Sandbox2
    SOURCES
        ${SOURCES}
        ${HEADERS}
    INCLUDES
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/>
    LINKS
        ${LINKS}
    DEFINITIONS
        ${DEFINITIONS}
    RESSOURCES
        ${RESSOURCES}
)
