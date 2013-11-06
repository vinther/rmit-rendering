add_executable (test_file_system file_manager_test.cpp)

target_link_libraries(test_file_system client ${Boost_LIBRARIES})