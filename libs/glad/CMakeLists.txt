set(LibName Glad)

add_library(${LibName} SHARED src/glad.c include/glad/glad.h include/KHR/khrplatform.h)
target_link_libraries(${LibName} PUBLIC glfw3)

if(NOT glfw3_FOUND)
	add_dependencies(Glad glfw3_project)
endif()

target_include_directories(${LibName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
