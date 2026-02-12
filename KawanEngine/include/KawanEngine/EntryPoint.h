#pragma once

#include "Application.h"
#include "KawanEngine.h"

extern Application* CreateApplication();

int main()
{
	Application* app = CreateApplication();

	//Start user app and initialize engine if successful
	if (!app->Start())
		return -1;
	KawanEngine::Instance().Init();

	//Main loop
	/*while (KawanEngine::Instance()._isRunning)
	{
		if (!app->Run())
			return -1;
	}*/

	if (!app->Run())
		return -1;

	//Shutdown app and engine
	if (!app->Shutdown())
		return -1;
	KawanEngine::Instance().Shutdown();
}