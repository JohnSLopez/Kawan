#include "KawanEngine/Renderer/Vulkan/Renderer.h"
#include "KawanEngine/Renderer/Vulkan/Initializers.h"
#include <iostream>
#include <vector>

void Renderer::Init()
{
	std::cout << "Initializing renderer" << std::endl;

	//TODO: Move this into engine instead of Renderer
	SDL_Init(SDL_INIT_VIDEO);

	SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

	SDL_Window* _window = SDL_CreateWindow(
		"Kawan Engine",
		1920,
		1080,
		windowFlags
	);

	InitRenderer();
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
	
	InitDevice(_device, _instance);
}

void Renderer::InitSwapchain()
{

}

void Renderer::InitCommands()
{

}

void Renderer::InitSyncStructures()
{

}
