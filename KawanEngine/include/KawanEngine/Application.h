#pragma once
#include <iostream>

class Application
{
public:
	virtual bool Start() = 0;
	virtual bool Run() = 0;
	virtual bool Shutdown() = 0;
};