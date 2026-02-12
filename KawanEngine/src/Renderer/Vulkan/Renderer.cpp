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

//TODO: Move extensions initialization to its own function
// and add link to https://wiki.libsdl.org/SDL3/SDL_Vulkan_GetInstanceExtensions
void Renderer::InitRenderer()
{
	uint32_t instanceExtensionsCount{ 0 };
	const char* const* instanceExtensions{ SDL_Vulkan_GetInstanceExtensions(&instanceExtensionsCount) };


	int extensionCount = instanceExtensionsCount + 1;
	const char** extensions = (const char**)SDL_malloc(extensionCount * sizeof(const char*));
	extensions[0] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
	SDL_memcpy(&extensions[1], instanceExtensions, instanceExtensionsCount * sizeof(const char*));

	InitInstance(&_instance, "Vulkan Renderer", VK_API_VERSION_1_3, instanceExtensionsCount, extensions);

	SDL_free(extensions);
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
