#include "KawanEngine/KawanEngine.h"
#include <iostream>
#include <KawanEngine/Renderer/Vulkan/Renderer.h>
#include <glm/glm.hpp>

void KawanEngine::Init()
{
	std::cout << "Initializing Engine" << std::endl;

	_renderer = std::make_shared<Renderer>();
	_renderer->Init();
	_isRunning = true;
}

void KawanEngine::Shutdown()
{
	std::cout << "Shutting engine down" << std::endl;
	_renderer->Shutdown();
}

KW_EXPORT const glm::vec2& KawanEngine::GetWindowSize() const
{
	return glm::vec2(_windowWidth, _windowHeight);
}
