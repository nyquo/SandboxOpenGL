set(LibName Core)

add_library(${LibName} STATIC
  Logger.hpp
  Logger.cpp
)
target_include_directories(${LibName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})