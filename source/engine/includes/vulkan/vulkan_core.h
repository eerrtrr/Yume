#ifndef VULKAN_CORE_H
#define VULKAN_CORE_H

#include <vulkan/vulkan.h>
#include <stdbool.h>

//Get instance
VkInstance getVulkanInstance();


//Vulkan init
bool initializeVulkan();

//Physical device selection
bool selectPhysicalDevice(VkInstance instance);
bool isDeviceSuitable(VkPhysicalDevice device);
bool checkQueueFamilySupport(VkPhysicalDevice device);

//Cleanup
void cleanupVulkan();
#endif