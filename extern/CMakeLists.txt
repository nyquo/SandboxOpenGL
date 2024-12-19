option(CMAKE_TLS_VERIFY "Verify SSL certificates" ON)

# # don't repeatedly build ExternalProjects.
# set_property(DIRECTORY PROPERTY EP_UPDATE_DISCONNECTED true)

########
# GLFW #
########

FetchContent_Declare(
    glfw3
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579
)

FetchContent_MakeAvailable(glfw3)

#######
# GLM #
#######

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
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
