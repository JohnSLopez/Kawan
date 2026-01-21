#pragma once

#include "Application.h"

extern Application* CreateApplication();

int main()
{
	Application* app = CreateApplication();

	if (!app->Start())
		return -1;

	if (!app->Run())
		return -1;

	if (!app->Shutdown())
		return -1;
}