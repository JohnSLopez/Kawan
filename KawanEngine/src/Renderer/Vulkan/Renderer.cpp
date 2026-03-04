#include "KawanEngine/Renderer/Vulkan/Renderer.h"
#include "KawanEngine/Renderer/Vulkan/Initializers.h"
#include "KawanEngine/KawanEngine.h"
#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vector>
#include <glm/glm.hpp>

void Renderer::Init()
{
	std::cout << "Initializing renderer" << std::endl;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

	//TODO: Get window information from Application
	_window = SDL_CreateWindow(
		"Kawan Engine",
		KawanEngine::Instance().GetWindowSize().x,
		KawanEngine::Instance().GetWindowSize().y,
		windowFlags
	);

	InitRenderer();
}

void Renderer::Update()
{
	SDL_Event e;
	SDL_PollEvent(&e);
	
	if (e.type == SDL_EVENT_QUIT)
	{
		KawanEngine::Instance().SetIsRunning(false);
	}
}

void Renderer::Shutdown()
{
	std::cout << "Shutting down renderer" << std::endl;
}

void Renderer::InitRenderer()
{
	//Initialize instance with extensions
	std::vector<const char*> userExtensions = { VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME };
	ExtensionInitializer extensions(userExtensions);
	InitInstance(&_instance, "Vulkan Renderer", VK_API_VERSION_1_3, extensions.GetExtensionCount(), extensions.GetExtensions());
	
	InitDevice(_device, _userGPU, _instance, _queue);
	InitVMA(_allocator, _userGPU, _device, _instance);

	Check(SDL_Vulkan_CreateSurface(_window, _instance, nullptr, &_surface));
	Check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_userGPU, _surface, &_surfaceCapabilities));

	VkExtent2D extent
	{
		KawanEngine::Instance().GetWindowSize().x,
		KawanEngine::Instance().GetWindowSize().y
	};
	VkSwapchainCreateInfoKHR swapchainCI = InitSwapchain(VK_FORMAT_B8G8R8A8_SRGB, _surface, _surfaceCapabilities, extent);
	Check(vkCreateSwapchainKHR(_device, &swapchainCI, nullptr, &_swapchain));

	//Depth image create info
	glm::vec2 windowSize = glm::vec2(KawanEngine::Instance().GetWindowSize().x, KawanEngine::Instance().GetWindowSize().y);
	VkImageCreateInfo depthImageCI = InitDepthAttachment(_userGPU, windowSize);

	VmaAllocationCreateInfo allocCI = {};
	allocCI.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
	allocCI.usage = VMA_MEMORY_USAGE_AUTO;

	//allocator && pImageCreateInfo && pAllocationCreateInfo && pImage && pAllocation
	Check(vmaCreateImage(_allocator, &depthImageCI, &allocCI, &_depthImage, &_depthImageAllocation, nullptr));

	//depth image view creation
	VkImageViewCreateInfo depthViewCI = InitDepthAttachmentView(_depthImage, depthImageCI.format);
	vkCreateImageView(_device, &depthViewCI, nullptr, &_depthImageView);

}

void Renderer::InitCommands()
{

}

void Renderer::InitSyncStructures()
{

}
