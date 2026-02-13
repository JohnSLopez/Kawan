#define VMA_IMPLEMENTATION
#include "KawanEngine/Renderer/Vulkan/Initializers.h"

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

//TODO: Refactor to add options for features/queues
void InitDevice(VkDevice& device, VkInstance& instance, VkQueue& queue)
{
	uint32_t deviceCount{ 0 };
	Check(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr));
	std::vector<VkPhysicalDevice> devices(deviceCount);
	Check(vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()));

	VkPhysicalDeviceProperties2 deviceProperties
	{
		deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
		deviceProperties.pNext = nullptr
	};
	vkGetPhysicalDeviceProperties2(devices[0], &deviceProperties);

	uint32_t queueFamilyCount{ 0 };
	vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &queueFamilyCount, queueFamilies.data());
	uint32_t queueFamily{ 0 };
	for (size_t i = 0; i < queueFamilies.size(); i++)
	{
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			queueFamily = i;
			break;
		}
	}
	Check(SDL_Vulkan_GetPresentationSupport(instance, devices[0], queueFamily));

	const float queuePriorities{ 1.0f };
	VkDeviceQueueCreateInfo queueCreateInfo
	{
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		queueCreateInfo.pNext = nullptr,
		queueCreateInfo.flags = NULL,
		queueCreateInfo.queueFamilyIndex = queueFamily,
		queueCreateInfo.queueCount = 1,
		queueCreateInfo.pQueuePriorities = &queuePriorities
	};

	const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkPhysicalDeviceVulkan12Features enabledVk12Features{
		enabledVk12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		enabledVk12Features.pNext = nullptr,
		enabledVk12Features.descriptorIndexing = true,
		enabledVk12Features.shaderSampledImageArrayNonUniformIndexing = true,
		enabledVk12Features.descriptorBindingVariableDescriptorCount = true,
		enabledVk12Features.runtimeDescriptorArray = true,
		enabledVk12Features.bufferDeviceAddress = true
	};

	VkPhysicalDeviceVulkan13Features enabledVk13Features
	{
		enabledVk13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		enabledVk13Features.pNext = &enabledVk12Features,
		enabledVk13Features.synchronization2 = true,
		enabledVk13Features.dynamicRendering = true
	};

	VkPhysicalDeviceFeatures enabledVk10Features =
	{
		enabledVk10Features.samplerAnisotropy = VK_TRUE
	};

	VkDeviceCreateInfo deviceCreateInfo
	{
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		deviceCreateInfo.pNext = &enabledVk13Features,
		deviceCreateInfo.flags = NULL,
		deviceCreateInfo.queueCreateInfoCount = 1,
		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo,
		deviceCreateInfo.enabledLayerCount = NULL,
		deviceCreateInfo.ppEnabledLayerNames = NULL,
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data(),
		deviceCreateInfo.pEnabledFeatures = &enabledVk10Features
	};

	Check(vkCreateDevice(devices[0], &deviceCreateInfo, nullptr, &device));
	vkGetDeviceQueue(device, queueFamily, 0, &queue);
}

void InitVMA(VmaAllocator allocator, VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance)
{
	VmaVulkanFunctions vkFunctions;
	vkFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
	vkFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
	vkFunctions.vkCreateImage = vkCreateImage;

	VmaAllocatorCreateInfo allocatorCreateInfo;
	allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
	allocatorCreateInfo.physicalDevice = physicalDevice;
	allocatorCreateInfo.device = device;
	allocatorCreateInfo.pVulkanFunctions = &vkFunctions;
	allocatorCreateInfo.instance = instance;
	//allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_4;

	Check(vmaCreateAllocator(&allocatorCreateInfo, &allocator));
}