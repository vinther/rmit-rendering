find_package(Boost COMPONENTS program_options REQUIRED)

add_executable (octree Octree.cpp)

message(${Boost_LIBRARIES})

target_link_libraries(
	octree 
	client
	
	${Boost_LIBRARIES}
	)