cmake_minimum_required(VERSION 4.0)

project(Kawan)

include(FetchContent)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)

set(ENGINE_TARGET_NAME KawanEngine)
set(TEST_TARGET_NAME test)
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")

FetchContent_Declare(
    sdl
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
)

FetchContent_MakeAvailable(sdl)

add_subdirectory(KawanEngine)
add_subdirectory(testbed)

target_link_libraries(${TEST_TARGET_NAME} ${ENGINE_TARGET_NAME})