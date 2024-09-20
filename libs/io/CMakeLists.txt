set(LibName Io)

file(GLOB_RECURSE Headers
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

file(GLOB_RECURSE Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

add_library(${LibName} INTERFACE
        ${Headers}
        ${Sources}
)

target_include_directories(${LibName} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
