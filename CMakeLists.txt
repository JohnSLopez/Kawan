cmake_minimum_required(VERSION 4.0)

project(Kawan)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Executable)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Executable)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Executable)

set(ENGINE_TARGET_NAME KawanEngine)
set(TEST_TARGET_NAME test)

add_subdirectory(KawanEngine)
add_subdirectory(testbed)

target_link_libraries(${TEST_TARGET_NAME} ${ENGINE_TARGET_NAME})