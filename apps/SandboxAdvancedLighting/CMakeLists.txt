set(RESSOURCES_FOLDER ${AppName}Ressources)

# file(GLOB MODELS
#     CONFIGURE_DEPENDS
#     RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
#     Models/backpack/*
# )

file(GLOB TEXTURES
    CONFIGURE_DEPENDS
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    textures/*
)

sogl_add_executable(SandboxAdvancedLighting
    SOURCES
        main.cpp
        CameraMover.hpp
        EditorLayer.cpp
        EditorLayer.hpp
        LightingScene.cpp
        LightingScene.hpp
        Sandbox.cpp
        Sandbox.hpp
        TrackballCameraMover.hpp
        TrackballCameraMover.cpp
    INCLUDES
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/>
    LINKS
        Core
        Glad
        Renderer
        glm::glm
        imgui
        Utils
    RESSOURCES
        shaders/modelShader.frag
        shaders/modelShader.vert
        shaders/pointLightCube.frag
        shaders/pointLightCube.geom
        shaders/pointLightCube.vert
        # ${BACKPACK_MODEL}
        ${TEXTURES}
    DEFINITIONS
        RESSOURCES_FOLDER="${RESSOURCES_FOLDER}"
)