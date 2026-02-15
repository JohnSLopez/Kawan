#define VMA_IMPLEMENTATION
#include "KawanEngine/Renderer/Vulkan/Initializers.h"
#include <SDL3/SDL_vulkan.h>

static inline void Check(VkResult result)
{
	if (result != VK_SUCCESS) {
		std::cerr << "Vulkan call returned an error (" << result << ")\n";
		exit(result);
	}
}

static inline void Check(bool result)
{
	if (result != true) {
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

//TODO: Refactor to add options for features/queues
void InitDevice(VkDevice& device, VkPhysicalDevice& physicalDevice, VkInstance& instance, VkQueue& queue)
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

	physicalDevice = devices[0];
	Check(vkCreateDevice(devices[0], &deviceCreateInfo, nullptr, &device));
	vkGetDeviceQueue(device, queueFamily, 0, &queue);
}

void InitVMA(VmaAllocator allocator, VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance)
{
	//For some reason, every member of the vma structs need to be manually initialized
	//thanks to: https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/issues/56
	VmaVulkanFunctions vkFunctions;
	vkFunctions.vkAllocateMemory = vkAllocateMemory;
	vkFunctions.vkBindBufferMemory = vkBindBufferMemory;
	vkFunctions.vkBindImageMemory = vkBindImageMemory;
	vkFunctions.vkCmdCopyBuffer = vkCmdCopyBuffer;
	vkFunctions.vkCreateBuffer = vkCreateBuffer;
	vkFunctions.vkCreateImage = vkCreateImage;
	vkFunctions.vkDestroyBuffer = vkDestroyBuffer;
	vkFunctions.vkDestroyImage = vkDestroyImage;
	vkFunctions.vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges;
	vkFunctions.vkFreeMemory = vkFreeMemory;
	vkFunctions.vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements;
	vkFunctions.vkGetImageMemoryRequirements = vkGetImageMemoryRequirements;
	vkFunctions.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
	vkFunctions.vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties;
	vkFunctions.vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges;
	vkFunctions.vkMapMemory = vkMapMemory;
	vkFunctions.vkUnmapMemory = vkUnmapMemory;
	vkFunctions.vkGetBufferMemoryRequirements2KHR = 0;  //(PFN_vkGetBufferMemoryRequirements2KHR)vkGetBufferMemoryRequirements2KHR;
	vkFunctions.vkGetImageMemoryRequirements2KHR = 0;  //(PFN_vkGetImageMemoryRequirements2KHR)vkGetImageMemoryRequirements2KHR;

	VmaAllocatorCreateInfo allocatorCreateInfo;
	allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
	allocatorCreateInfo.physicalDevice = physicalDevice;
	allocatorCreateInfo.device = device;
	allocatorCreateInfo.preferredLargeHeapBlockSize = 0;
	allocatorCreateInfo.pAllocationCallbacks = VMA_NULL;
	allocatorCreateInfo.pDeviceMemoryCallbacks = VMA_NULL;
	allocatorCreateInfo.pHeapSizeLimit = VMA_NULL;
	allocatorCreateInfo.pVulkanFunctions = &vkFunctions;
	allocatorCreateInfo.instance = instance;
	allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_4;
	allocatorCreateInfo.pTypeExternalMemoryHandleTypes = VMA_NULL;

	Check(vmaCreateAllocator(&allocatorCreateInfo, &allocator));
}

ExtensionInitializer::ExtensionInitializer(const std::vector<const char*>& userExtensions)
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

ExtensionInitializer::~ExtensionInitializer()
{
	SDL_free(extensions);
}

const char** ExtensionInitializer::GetExtensions() const
{
	return extensions;
}

const int ExtensionInitializer::GetExtensionCount() const
{
	return extensionCount;
}
