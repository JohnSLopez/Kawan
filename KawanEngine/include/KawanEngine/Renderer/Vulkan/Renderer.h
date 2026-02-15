#pragma once

#include "defines.h"
#include <memory>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

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

	void InitRenderer();
	void InitSwapchain();
	void InitCommands();
	void InitSyncStructures();
};