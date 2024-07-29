set(AppName HelloWord)

set(Sources
	main.cpp
)

set(Headers "")

set(Links
	Core
	Glad
)

add_executable(${AppName} ${Sources} ${Headers})
target_link_libraries(${AppName} PRIVATE ${Links})
target_include_directories(${AppName} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})