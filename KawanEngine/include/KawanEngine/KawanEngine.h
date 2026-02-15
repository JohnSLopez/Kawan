#pragma once

#include "defines.h"
#include <memory>
#include <glm/fwd.hpp>

class Renderer;

class KawanEngine
{
public:
	KW_EXPORT static KawanEngine& Instance()
	{
		static KawanEngine _instance;
		return _instance;
	}

	KW_EXPORT void Init();
	KW_EXPORT void Shutdown();

	KW_EXPORT const glm::vec2& GetWindowSize() const;

private:
	uint16_t _windowWidth = 1920 / 2;
	uint16_t _windowHeight = 1080 / 2;
	bool _isInitialized{ false };
	bool _isRunning{ false };

	std::shared_ptr<Renderer> _renderer;

	KawanEngine() {}
};