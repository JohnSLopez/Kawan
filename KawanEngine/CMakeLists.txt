set(ENGINE_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/EngineTest/EngineTest.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/EngineTest.cpp"
)

set(ENGINE_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_library(${ENGINE_TARGET_NAME} SHARED ${ENGINE_SRCS})
target_include_directories(${ENGINE_TARGET_NAME} PUBLIC ${ENGINE_INCLUDE_DIRS})