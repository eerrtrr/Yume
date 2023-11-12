#ifndef VULKAN_CORE_H
#define VULKAN_CORE_H

#include <vulkan/vulkan.h>
#include <stdbool.h>
#include "../utils/vulkan_structs.h"

//Get instance
VkInstance getVulkanInstance();


//Vulkan init
bool initializeVulkan();

//Physical device selection
bool selectPhysicalDevice(VkInstance instance);
bool isDeviceSuitable(VkPhysicalDevice physicalDevice);

//Queue Family checking and identification
bool checkQueueFamilySupport(VkPhysicalDevice physicalDevice);
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice);

//Logical device creation
bool createLogicalDevice(VkPhysicalDevice physicalDevice, VkDevice* logicalDevice);

//Cleanup
void cleanupVulkan();
#endif