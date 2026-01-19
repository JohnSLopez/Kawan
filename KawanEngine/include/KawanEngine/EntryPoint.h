#pragma once

#include "Application.h"

extern Application* CreateApplication();

int main()
{
	Application* app = CreateApplication();

	if (!app->Start())
		return -1;
}