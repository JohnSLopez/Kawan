add_library(test SHARED test.cpp)
target_include_directories(test INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})