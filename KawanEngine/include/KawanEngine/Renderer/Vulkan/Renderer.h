#pragma once

#include "defines.h"
#include <memory>
#include <vma/vk_mem_alloc.h>

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <glm/glm.hpp>

struct SDL_Window;

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Renderer
{
public:
	void Init();
	void Update();
    void Shutdown();

private:
	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debugMessenger;
	VkPhysicalDevice _userGPU;
	VkDevice _device;
	VkSurfaceKHR _surface;
	VkQueue _queue;
	VmaAllocator _allocator;
	VkSurfaceCapabilitiesKHR _surfaceCapabilities;
	VkSwapchainKHR _swapchain;
	SDL_Window* _window;

	VkImage _depthImage;
	VmaAllocation _depthImageAllocation;
	VkImageView _depthImageView;

	void InitRenderer();
	void InitCommands();
	void InitSyncStructures();
};