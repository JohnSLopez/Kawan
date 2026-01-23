#pragma once

#include "defines.h"

class Renderer
{
public:
	KW_EXPORT static Renderer& Instance()
	{
		static Renderer _instance;
		return _instance;
	}

	KW_EXPORT void Init();
	KW_EXPORT void Shutdown();
};