#include "TestApp.h"
#include <iostream>

bool TestApp::Start()
{
	std::cout << "Starting Application" << std::endl;
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
	std::cout << "Shutting Application Down" << std::endl;
	return true;
}

Application* CreateApplication()
{
	TestApp* userApp = new TestApp;
	return userApp;
}