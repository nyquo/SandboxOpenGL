set(LibName Core)

add_library(${LibName}
   Events/Event.hpp
   Events/WindowEvent.hpp
   gl.h
   Logger.hpp
   Logger.cpp
   Shader.hpp
   Shader.cpp
)

target_link_libraries(${LibName} PRIVATE Glad)
target_include_directories(${LibName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})