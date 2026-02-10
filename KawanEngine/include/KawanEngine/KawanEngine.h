#pragma once

#include "defines.h"

class KawanEngine
{
public:
	bool _isInitialized { false };
	bool _isRunning{ false };

	KW_EXPORT static KawanEngine& Instance()
	{
		static KawanEngine _instance;
		return _instance;
	}

	KW_EXPORT void Init();
	KW_EXPORT void Shutdown();

private:
	KawanEngine() {}
};