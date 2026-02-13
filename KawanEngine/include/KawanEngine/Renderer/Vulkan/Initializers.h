#pragma once

#include <cstdint>
#include <iostream>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

//#define VMA_VULKAN_VERSION 1003000
//#define VMA_STATIC_VULKAN_FUNCTIONS 0
//#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include <vma/vk_mem_alloc.h>
#include <vector>

static inline void Check(VkResult result) {
	if (result != VK_SUCCESS) {
		std::cerr << "Vulkan call returned an error (" << result << ")\n";
		exit(result);
	}
}

static inline void Check(bool result) {
	if (result != true) {
		std::cerr << "Vulkan call returned an error (" << result << ")\n";
		exit(result);
	}
}

void InitInstance(VkInstance* instance, const char* appName, uint32_t apiVersion, uint32_t enabledExtensionCount, const char* const* ppEnabledExtensionNames);

void InitDevice(VkDevice& device, VkInstance& instance, VkQueue& queue);

void InitVMA(VmaAllocator allocator, VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance);

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