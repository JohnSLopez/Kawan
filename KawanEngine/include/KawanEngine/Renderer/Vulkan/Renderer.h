#pragma once

#include "defines.h"
#include <memory>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

struct SDL_Window;

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

	void InitRenderer();
	void InitCommands();
	void InitSyncStructures();
};