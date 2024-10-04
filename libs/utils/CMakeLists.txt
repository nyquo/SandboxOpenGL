set(LibName Utils)

file(GLOB_RECURSE Headers
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

file(GLOB_RECURSE Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

set(PublicLinks
    glm::glm
    imgui
)


add_library(${LibName} INTERFACE
        ${Headers}
        ${Sources}
)

target_link_libraries(${LibName} INTERFACE ${PublicLinks})
target_include_directories(${LibName} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
