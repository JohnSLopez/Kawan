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
	VkApplicationInfo appInfo
	{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = NULL,
		.pApplicationName = appName,
		.apiVersion = apiVersion
	};

	VkInstanceCreateInfo instanceCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = NULL,
		.flags = VkInstanceCreateFlagBits::VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = enabledExtensionCount,
		.ppEnabledExtensionNames = ppEnabledExtensionNames
	};

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
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
		.pNext = nullptr
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
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.flags = NULL,
		.queueFamilyIndex = queueFamily,
		.queueCount = 1,
		.pQueuePriorities = &queuePriorities
	};

	const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkPhysicalDeviceVulkan12Features enabledVk12Features
	{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		.pNext = nullptr,
		.descriptorIndexing = true,
		.shaderSampledImageArrayNonUniformIndexing = true,
		.descriptorBindingVariableDescriptorCount = true,
		.runtimeDescriptorArray = true,
		.bufferDeviceAddress = true
	};

	VkPhysicalDeviceVulkan13Features enabledVk13Features
	{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.pNext = &enabledVk12Features,
		.synchronization2 = true,
		.dynamicRendering = true
	};

	VkPhysicalDeviceFeatures enabledVk10Features
	{
		.samplerAnisotropy = VK_TRUE
	};

	VkDeviceCreateInfo deviceCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = &enabledVk13Features,
		.flags = NULL,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &queueCreateInfo,
		.enabledLayerCount = NULL,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
		.ppEnabledExtensionNames = deviceExtensions.data(),
		.pEnabledFeatures = &enabledVk10Features
	};

	physicalDevice = devices[0];
	Check(vkCreateDevice(devices[0], &deviceCreateInfo, nullptr, &device));
	vkGetDeviceQueue(device, queueFamily, 0, &queue);
}

void InitVMA(VmaAllocator& allocator, VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance)
{
	VmaVulkanFunctions vkFunctions
	{
		.vkGetInstanceProcAddr = &vkGetInstanceProcAddr,
		.vkGetDeviceProcAddr = &vkGetDeviceProcAddr
	};

	VmaAllocatorCreateInfo allocatorCreateInfo
	{
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = physicalDevice,
		.device = device,
		.pVulkanFunctions = &vkFunctions,
		.instance = instance
	};

	Check(vmaCreateAllocator(&allocatorCreateInfo, &allocator));
}

VkSwapchainCreateInfoKHR InitSwapchain(
	const VkFormat imageFormat, 
	VkSurfaceKHR& surface, 
	VkSurfaceCapabilitiesKHR surfaceCapabilities, 
	VkExtent2D extent)
{
	VkSwapchainCreateInfoKHR swapchainCI
	{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.pNext = nullptr,
		.flags = NULL,
		.surface = surface,
		.minImageCount = surfaceCapabilities.minImageCount,
		.imageFormat = imageFormat,
		.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
		.imageExtent = extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = NULL,
		.pQueueFamilyIndices = NULL,
		.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = VK_PRESENT_MODE_FIFO_KHR,
		.clipped = VK_TRUE,
		.oldSwapchain = VK_NULL_HANDLE,
	};


	//VSync mode guaranteed to be available everywhere
	swapchainCI.presentMode = VK_PRESENT_MODE_FIFO_KHR;
	return swapchainCI;
}

VkImageViewCreateInfo InitDepthAttachmentView(const VkImage& depthImage, const VkFormat& depthFormat)
{
	VkImageViewCreateInfo depthViewCI
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = depthImage,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = depthFormat
	};

	VkImageSubresourceRange subresourceRange
	{
		.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
		.levelCount = 1,
		.layerCount = 1
	};
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

	VkImageCreateInfo depthImageCI 
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = depthFormat,

		.extent 
		{
			 .width = static_cast<uint32_t>(windowSize.x),
			 .height = static_cast<uint32_t>(windowSize.y),
			 .depth = 1
		},

		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
	};
	
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
