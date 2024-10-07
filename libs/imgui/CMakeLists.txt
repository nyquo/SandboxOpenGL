set(LibName imgui)

file(GLOB_RECURSE Headers
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

file(GLOB_RECURSE Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

if(WIN32)
        add_library(${LibName} STATIC
                ${Headers}
                ${Sources}
        )
else()
        add_library(${LibName} STATIC
                ${Headers}
                ${Sources}
        )
endif()


target_include_directories(${LibName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LibName} PRIVATE Glad)
