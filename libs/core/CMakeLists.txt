set(HEADERS
    Events/Event.hpp
    Events/KeyEvent.hpp
    Events/MouseEvent.hpp
    Events/WindowEvent.hpp
    Application.hpp
    CoreExport.hpp
    gl.h
    Input.hpp
    Layer.hpp
    LayerStack.hpp
    Logger.hpp
    Window.hpp
)

set(SOURCES
    Application.cpp
    LayerStack.cpp
    Logger.cpp
    Window.cpp
)

set(PRIVATE_LINKS
    Glad
)

set(PUBLIC_LINKS
    glm::glm
    imgui
)

sogl_add_library(Core
    SOURCES
        ${SOURCES}
    HEADERS
        ${HEADERS}
    PUBLIC_INCLUDES
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libs>
        $<INSTALL_INTERFACE:include/>
    PRIVATE_INCLUDES
    PUBLIC_LINKS
        ${PUBLIC_LINKS}
    PRIVATE_LINKS
        ${PRIVATE_LINKS}
    PUBLIC_DEFINITIONS
    PRIVATE_DEFINITIONS
    RESSOURCES
)

