#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

void Check(VkResult result);
void Check(bool result);

void InitInstance(VkInstance* instance, const char* appName, uint32_t apiVersion, uint32_t enabledExtensionCount, const char* const* ppEnabledExtensionNames);
void InitDevice(VkDevice& device, VkPhysicalDevice& physicalDevice, VkInstance& instance, VkQueue& queue);
void InitVMA(VmaAllocator allocator, VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance);
VkSwapchainCreateInfoKHR InitSwapchain(
	const VkFormat imageFormat, 
	VkSurfaceKHR& surface, 
	VkSurfaceCapabilitiesKHR surfaceCapabilities, 
	VkExtent2D extent);

//Initializes extensions provided by user along with necessary extensions grabbed by SDL
// 
//https://wiki.libsdl.org/SDL3/SDL_Vulkan_GetInstanceExtensions
class ExtensionInitializer
{
public:
	ExtensionInitializer(const std::vector<const char*>& userExtensions);
	~ExtensionInitializer();

	const char** GetExtensions() const;
	const int GetExtensionCount() const;

private:
	const char** extensions;
	int extensionCount = 0;
};