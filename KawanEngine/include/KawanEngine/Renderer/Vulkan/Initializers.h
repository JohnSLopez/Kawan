#pragma once
#include <cstdint>
#include <vulkan/vulkan.h>
#include <iostream>

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

//void InitExtensions