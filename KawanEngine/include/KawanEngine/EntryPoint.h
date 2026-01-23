#pragma once

#include "Application.h"
#include "KawanEngine.h"

extern Application* CreateApplication();

int main()
{
	Application* app = CreateApplication();

	if (!app->Start())
		return -1;
	KawanEngine::Instance().Init();

	if (!app->Run())
		return -1;

	if (!app->Shutdown())
		return -1;
	KawanEngine::Instance().Shutdown();
}