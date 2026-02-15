#pragma once

#include "defines.h"
#include <memory>

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

private:
	bool _isInitialized{ false };
	bool _isRunning{ false };

	std::shared_ptr<Renderer> _renderer;

	KawanEngine() {}
};