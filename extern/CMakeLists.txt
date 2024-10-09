option(CMAKE_TLS_VERIFY "Verify SSL certificates" ON)

# don't repeatedly build ExternalProjects.
set_property(DIRECTORY PROPERTY EP_UPDATE_DISCONNECTED true)

########
# GLFW #
########

set(GLFW_VERSION 3.3) 

find_package(glfw3 ${GLFW_VERSION} QUIET)
if(NOT glfw3_FOUND)
	ExternalProject_Add(
		glfw3_project
		PREFIX ${CMAKE_BINARY_DIR}/glfw
		GIT_REPOSITORY https://github.com/glfw/glfw.git
		GIT_TAG ${GLFW_VERSION}
		CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/glfw/install
		CONFIGURE_HANDLED_BY_BUILD ON
		BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/glfw/install/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}
	)
	
	# Add the install directory of GLFW to CMake's search path
    ExternalProject_Get_Property(glfw3_project INSTALL_DIR)
	message(STATUS "glfw3 install dir: ${INSTALL_DIR}")
    set(GLFW_INCLUDE_DIR ${INSTALL_DIR}/install/include)
    set(GLFW_LIBRARY ${INSTALL_DIR}/install/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX})

	# Manually specify the include directories and libraries for GLFW
    add_library(glfw3 STATIC IMPORTED GLOBAL)
    set_target_properties(glfw3 PROPERTIES
        IMPORTED_LOCATION ${GLFW_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${GLFW_INCLUDE_DIR}
    )

	add_dependencies(glfw3 glfw3_project)
else()
	message(STATUS "Found GLFW ${glfw3_VERSION}")
	message(STATUS "GLFW dir: ${glfw3_DIR}")
endif()

set(glfw3_FOUND ${glfw3_FOUND} PARENT_SCOPE)

#######
# GLM #
#######

FetchContent_Declare(
    glm
    GIT_REPOSITORY	https://github.com/g-truc/glm.git
    GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

##########
# Assimp #
##########

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG c35200e38ea8f058812b83de2ef32c6093b0ece2 #refs/tags/v5.4.3
)

FetchContent_MakeAvailable(assimp)
