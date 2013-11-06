add_executable (test_data_structures indata_free_list_test.cpp)

target_link_libraries(test_data_structures client ${Boost_LIBRARIES})