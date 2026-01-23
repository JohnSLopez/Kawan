#include "KawanEngine/KawanEngine.h"
#include <iostream>
#include <KawanEngine/Renderer/Vulkan/Renderer.h>

void KawanEngine::Init()
{
	std::cout << "Initializing Engine" << std::endl;
	Renderer::Instance().Init();
}

void KawanEngine::Shutdown()
{
	std::cout << "Shutting engine down" << std::endl;
	Renderer::Instance().Shutdown();
}
