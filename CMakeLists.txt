cmake_minimum_required(VERSION 4.0)

project(Kawan)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} test)
add_subdirectory(engine)