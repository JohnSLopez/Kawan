#include "TestApp.h"
#include <iostream>

bool TestApp::Start()
{
	std::cout << "Starting Application" << std::endl;
	return true;
}

//TODO: Move this function into engine and make into a template
Application* CreateApplication()
{
	TestApp* userApp = new TestApp;
	return userApp;
}