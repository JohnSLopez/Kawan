#define VMA_IMPLEMENTATION
#include "KawanEngine/Renderer/Vulkan/Initializers.h"
#include <SDL3/SDL_vulkan.h>
#include <glm/glm.hpp>

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1

//TODO: Remove Vk and Vma create functions so user can adjust settings before calling them themselves

void Check(VkResult result)
{
	if (result != VK_SUCCESS) {
		std::cerr << "Vulkan call returned an error (" << result << ")\n";
		exit(result);
	}
}

void Check(bool result)
{
	if (result != true) {
		std::cerr << "Vulkan call returned an error (" << result << ")\n";
		exit(result);
	}
}

void InitInstance(VkInstance* instance, const char* appName, uint32_t apiVersion, uint32_t enabledExtensionCount, const char* const* ppEnabledExtensionNames)
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	appInfo.pNext = NULL,
	appInfo.pApplicationName = appName,
	appInfo.apiVersion = apiVersion;

	VkInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	instanceCreateInfo.pNext = NULL,
	instanceCreateInfo.flags = VkInstanceCreateFlagBits::VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
	instanceCreateInfo.pApplicationInfo = &appInfo,
	instanceCreateInfo.enabledLayerCount = 0,
	instanceCreateInfo.ppEnabledLayerNames = NULL,
	instanceCreateInfo.enabledExtensionCount = enabledExtensionCount,
	instanceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames;

	vkCreateInstance(&instanceCreateInfo, nullptr, instance);
	//Check(vkCreateInstance(&instanceCreateInfo, nullptr, instance));
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

void InitVMA(VmaAllocator& allocator, VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance)
{
	VmaVulkanFunctions vkFunctions = {};
	vkFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
	vkFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

	VmaAllocatorCreateInfo allocatorCreateInfo = {};
	allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
	allocatorCreateInfo.physicalDevice = physicalDevice;
	allocatorCreateInfo.device = device;
	allocatorCreateInfo.pVulkanFunctions = &vkFunctions;
	allocatorCreateInfo.instance = instance;

	Check(vmaCreateAllocator(&allocatorCreateInfo, &allocator));
}

VkSwapchainCreateInfoKHR InitSwapchain(
	const VkFormat imageFormat, 
	VkSurfaceKHR& surface, 
	VkSurfaceCapabilitiesKHR surfaceCapabilities, 
	VkExtent2D extent)
{
	VkSwapchainCreateInfoKHR swapchainCI;
	swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCI.pNext = nullptr;
	swapchainCI.flags = NULL;
	swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCI.clipped = VK_TRUE;
	swapchainCI.oldSwapchain = VK_NULL_HANDLE;
	swapchainCI.surface = surface;
	swapchainCI.minImageCount = surfaceCapabilities.minImageCount;
	swapchainCI.imageFormat = imageFormat;
	swapchainCI.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	swapchainCI.imageExtent = extent;
	swapchainCI.imageArrayLayers = 1;
	swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCI.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapchainCI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	//VSync mode guaranteed to be available everywhere
	swapchainCI.presentMode = VK_PRESENT_MODE_FIFO_KHR;
	return swapchainCI;
}

VkImageViewCreateInfo InitDepthAttachmentView(const VkImage& depthImage, const VkFormat& depthFormat)
{
	VkImageViewCreateInfo depthViewCI = {};
	depthViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	depthViewCI.image = depthImage;
	depthViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
	depthViewCI.format = depthFormat;

	VkImageSubresourceRange subresourceRange = {};
	subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	subresourceRange.levelCount = 1;
	subresourceRange.layerCount = 1;
	depthViewCI.subresourceRange = subresourceRange;

	return depthViewCI;
}

VkImageCreateInfo InitDepthAttachment(const VkPhysicalDevice& userGpu, glm::vec2 windowSize)
{
	std::vector<VkFormat> depthFormats{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
	VkFormat depthFormat{ VK_FORMAT_UNDEFINED };

	for (VkFormat& format : depthFormats)
	{
		VkFormatProperties2 formatProperties2;
		formatProperties2.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2;
		formatProperties2.pNext = nullptr;

		VkFormatProperties formatProperties = {};
		formatProperties2.formatProperties = formatProperties;


		vkGetPhysicalDeviceFormatProperties2(userGpu, format, &formatProperties2);
		if (formatProperties2.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
		{
			depthFormat = format;
			break;
		}
	}

	VkImageCreateInfo depthImageCI = {};
	depthImageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	depthImageCI.imageType = VK_IMAGE_TYPE_2D;
	depthImageCI.format = depthFormat;

	VkExtent3D extent = {};
	extent.width = windowSize.x;
	extent.height = windowSize.y;
	extent.depth = 1;
	
	depthImageCI.extent = extent;
	depthImageCI.mipLevels = 1;
	depthImageCI.arrayLayers = 1;
	depthImageCI.samples = VK_SAMPLE_COUNT_1_BIT;
	depthImageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
	depthImageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	depthImageCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	return depthImageCI;
};
	

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
