include(CMakePrintHelpers)

find_package(Vulkan REQUIRED)

if(${Vulkan_FOUND})
    MESSAGE(STATUS "Found Vulkan")
else()
    MESSAGE(FATAL_ERROR "VUlkan not found, make sure the VUlkan SDK is installed")
endif()

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/**/*.h")
file(GLOB_RECURSE CPPFILES "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp")

set(ENGINE_SRCS
    "${HEADERS}"
    "${CPPFILES}"
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