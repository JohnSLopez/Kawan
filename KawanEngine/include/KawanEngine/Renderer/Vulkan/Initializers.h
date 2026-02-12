#pragma once
#include <cstdint>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

static inline void Check(VkResult result) {
	if (result != VK_SUCCESS) {
		std::cerr << "Vulkan call returned an error (" << result << ")\n";
		exit(result);
	}
}

void InitInstance(VkInstance* instance, const char* appName, uint32_t apiVersion, uint32_t enabledExtensionCount, const char* const* ppEnabledExtensionNames)
{
	VkApplicationInfo appInfo
	{
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		appInfo.pNext = NULL,
		appInfo.pApplicationName = appName,
		appInfo.apiVersion = apiVersion
	};

	VkInstanceCreateInfo instanceCreateInfo
	{
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		instanceCreateInfo.pNext = NULL,
		instanceCreateInfo.flags = VkInstanceCreateFlagBits::VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
		instanceCreateInfo.pApplicationInfo = &appInfo,
		instanceCreateInfo.enabledLayerCount = 0,
		instanceCreateInfo.ppEnabledLayerNames = NULL,
		instanceCreateInfo.enabledExtensionCount = enabledExtensionCount,
		instanceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames
	};

	Check(vkCreateInstance(&instanceCreateInfo, nullptr, instance));
}

//Initializes extensions provided by user along with necessary extensions grabbed by SDL
// 
//https://wiki.libsdl.org/SDL3/SDL_Vulkan_GetInstanceExtensions
class ExtensionInitializer
{
public:
	ExtensionInitializer(const std::vector<const char*>& userExtensions)
	{
		//Get extensions from SDL
		uint32_t instanceExtensionsCount{ 0 };
		const char* const* instanceExtensions{ SDL_Vulkan_GetInstanceExtensions(&instanceExtensionsCount) };

		//Allocate memory for extensions
		extensionCount = instanceExtensionsCount + userExtensions.size();
		extensions = (const char**)SDL_malloc(extensionCount * sizeof(const char*));

		//Add user defined extensions to beginning of extensions memory
		for (int i = 0; i < userExtensions.size(); i++)
		{
			extensions[i] = userExtensions[i];
		}

		//Copy SDL Instance extensions to the end of extensions memory
		SDL_memcpy(&extensions[userExtensions.size()], instanceExtensions, instanceExtensionsCount * sizeof(const char*));
	}

	~ExtensionInitializer()
	{
		SDL_free(extensions);
	}

	const char** GetExtensions() const
	{
		return extensions;
	}

	const int GetExtensionCount() const
	{
		return extensionCount;
	}

private:
	const char** extensions;
	int extensionCount = 0;
};