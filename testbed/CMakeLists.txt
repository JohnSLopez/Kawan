set(TEST_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

set(TEST_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(${TEST_TARGET_NAME} ${TEST_SRCS})
target_include_directories(${TEST_TARGET_NAME} PUBLIC ${TEST_INCLUDE_DIRS})