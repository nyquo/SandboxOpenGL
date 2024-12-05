set(LibName Glad)

sogl_add_library(${LibName}
    SOURCES
        src/glad.c
    HEADERS
        include/glad/glad.h
        include/KHR/khrplatform.h
    PUBLIC_INCLUDES
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libs>
        $<INSTALL_INTERFACE:include/>
    PRIVATE_INCLUDES
    PUBLIC_LINKS
    PRIVATE_LINKS
    PUBLIC_DEFINITIONS
    PRIVATE_DEFINITIONS
    RESSOURCES
)


# Ensure that glad is compiled with -fPIC
# set_target_properties(${LibName} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(NOT glfw3_FOUND)
	target_link_libraries(${LibName} PUBLIC glfw3)
	add_dependencies(Glad glfw3_project)
else()
        target_link_libraries(${LibName} PUBLIC glfw)
endif()
