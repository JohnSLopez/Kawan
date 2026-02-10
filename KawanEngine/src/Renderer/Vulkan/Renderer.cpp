#include "KawanEngine/Renderer/Vulkan/VkRenderer.h"
#include <iostream>
//#include <SDL3/SDL.h>
//#include <VkBootstrap.h>

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
}

void Renderer::Shutdown()
{
	std::cout << "Shutting down renderer" << std::endl;
}

void Renderer::InitRenderer()
{

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
