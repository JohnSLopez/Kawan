#include "KawanEngine/KawanEngine.h"
#include <iostream>
#include <KawanEngine/Renderer/Vulkan/Renderer.h>

void KawanEngine::Init()
{
	std::cout << "Initializing Engine" << std::endl;
	Renderer::Instance().Init();
	_isRunning = true;
}

void KawanEngine::Shutdown()
{
	std::cout << "Shutting engine down" << std::endl;
	Renderer::Instance().Shutdown();
}
