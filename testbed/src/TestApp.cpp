#include "TestApp.h"
#include <iostream>
#include <KawanEngine/KawanEngine.h>

bool TestApp::Start()
{
	std::cout << "Starting Application" << std::endl;

	KawanEngine::Instance().Init();

	return true;
}

bool TestApp::Run()
{
	//Put game loop here
	std::cout << "Running Application" << std::endl;
	return true;
}

bool TestApp::Shutdown()
{
	KawanEngine::Instance().Shutdown();
	std::cout << "Shutting Application Down" << std::endl;
	return true;
}

//TODO: Move this function into engine
Application* CreateApplication()
{
	TestApp* userApp = new TestApp;
	return userApp;
}