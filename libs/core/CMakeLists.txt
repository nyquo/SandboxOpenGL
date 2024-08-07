set(LibName Core)

add_library(${LibName}
   Events/Event.hpp
   Events/WindowEvent.hpp
   gl.h
   Logger.hpp
   Logger.cpp
)
target_include_directories(${LibName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})