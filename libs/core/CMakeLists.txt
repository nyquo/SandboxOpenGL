set(LibName Core)

file(GLOB_RECURSE Headers
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

file(GLOB_RECURSE Sources
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

set(PrivateLinks
    Glad
)

set(PublicLinks
    Io
    glm::glm
)

# For some reason building core shared produced runtime error with glad on windows
# As fighting we cmake and windows is not the purpose of this repo, this will be build 
# statically on windows for now
if(WIN32)
	add_library(${LibName} STATIC
		${Headers}
		${Sources}
	)

#	if(BUILD_SHARED_LIBS)
#		target_compile_definitions(Core PRIVATE CORE_API_EXPORT)
#	endif()
else()
	add_library(${LibName}
		${Headers}
		${Sources}
	)
endif()



target_link_libraries(${LibName} PRIVATE ${PrivateLinks})
target_link_libraries(${LibName} PUBLIC ${PublicLinks} )
target_include_directories(${LibName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
