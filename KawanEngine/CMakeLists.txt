include(CMakePrintHelpers)

find_package(Vulkan REQUIRED)

set(ENGINE_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/KawanEngine/KawanEngine.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/KawanEngine.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/KawanEngine/Renderer/Vulkan/Renderer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Vulkan/Renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/KawanEngine/Renderer/Vulkan/Initializers.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Vulkan/Initializers.cpp"
)

set(ENGINE_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${Vulkan_INCLUDE_DIRS}"
    "${THIRD_PARTY_DIR}/VkBootstrap"
    "${THIRD_PARTY_DIR}/vma"
)

set(ENGINE_LINK_DIR
    "Vulkan::Vulkan"
    "SDL3::SDL3"
)

cmake_print_variables(Vulkan_INCLUDE_DIRS)

add_library(${ENGINE_TARGET_NAME} SHARED ${ENGINE_SRCS})
target_include_directories(${ENGINE_TARGET_NAME} PUBLIC ${ENGINE_INCLUDE_DIRS})
target_link_libraries(${ENGINE_TARGET_NAME} PRIVATE ${ENGINE_LINK_DIR})