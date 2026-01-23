#pragma once

#include "defines.h"
#include <vulkan/vulkan.h>

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

class Renderer
{
public:
	KW_EXPORT static Renderer& Instance()
	{
		static Renderer _instance;
		return _instance;
	}

	KW_EXPORT void Init();
	KW_EXPORT void Shutdown();

private:
	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debugMessenger;
	VkPhysicalDevice _userGPU;
	VkDevice _device;
	VkSurfaceKHR _surface;
	//SDL_Window* _window;

	void InitVulkan();
	void InitSwapchain();
	void InitCommands();
	void InitSyncStructures();
};