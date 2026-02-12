#pragma once

#include "defines.h"
#include <vulkan/vulkan.h>

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

class Renderer
{
public:
	KW_EXPORT static Renderer& Instance()
	{
		static Renderer _rendererInstance;
		return _rendererInstance;
	}

	KW_EXPORT void Init();
	KW_EXPORT void Shutdown();

private:
	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debugMessenger;
	VkPhysicalDevice _userGPU;
	VkDevice _device;
	VkSurfaceKHR _surface;

	void InitRenderer();
	void InitSwapchain();
	void InitCommands();
	void InitSyncStructures();
};