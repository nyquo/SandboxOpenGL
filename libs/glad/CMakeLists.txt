set(LibName Glad)

add_library(${LibName} STATIC src/glad.c include/glad/glad.h include/KHR/khrplatform.h)

# Ensure that glad is compiled with -fPIC
set_target_properties(${LibName} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(NOT glfw3_FOUND)
	target_link_libraries(${LibName} PUBLIC glfw3)
	add_dependencies(Glad glfw3_project)
else()
        target_link_libraries(${LibName} PUBLIC glfw)
endif()

target_include_directories(${LibName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
