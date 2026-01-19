#include <KawanEngine/EntryPoint.h>
#include <iostream>

class TestApp : public Application
{
public:
	bool Start()
	{
		std::cout << "Starting Application" << std::endl;
		return true;
	}
};

Application* CreateApplication()
{
	std::cout << "Creating Application" << std::endl;
	TestApp* userApp = new TestApp;
	return userApp;
}