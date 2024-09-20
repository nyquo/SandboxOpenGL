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

add_library(${LibName}
	${Headers}
	${Sources}
)

if(BUILD_SHARED_LIBS)
	target_compile_definitions(Core PRIVATE BUILDING_DLL)
endif()

target_link_libraries(${LibName} PRIVATE ${PrivateLinks})
target_link_libraries(${LibName} PUBLIC ${PublicLinks} )
target_include_directories(${LibName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
