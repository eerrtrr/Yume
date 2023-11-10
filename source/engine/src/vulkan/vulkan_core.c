#include "../../includes/utils/logger.h"
#include "../../includes/vulkan/vulkan_core.h"


// Global variables to hold Vulkan objects
static VkInstance VK_Instance;
static VkPhysicalDevice VK_Physical_Device;

bool initializeVulkan(){
    VkApplicationInfo appInfo = {0};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Yume";
    appInfo.applicationVersion = VK_MAKE_VERSION(0,1,0);
    appInfo.pEngineName = "Yume_Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0,1,0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if(vkCreateInstance(&createInfo, NULL, &VK_Instance) != VK_SUCCESS){
        LOG_FATAL("Failed to create a Vulkan instance");
        return false;
    }

    LOG_INFO("Create Vulkan instance");

    selectPhysicalDevice(VK_Instance);

    cleanupVulkan();
    return true;
}

bool selectPhysicalDevice(VkInstance instance){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

    if(deviceCount == 0){
        LOG_FATAL("Failed to find GPUs with Vulkan support");
        return false;
    }

    VkPhysicalDevice devices[deviceCount];
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    //Pick the bette device
    for(uint32_t i=0; i<deviceCount; i++){
        if(isDeviceSuitable(devices[i])){
            VK_Physical_Device = devices[i];
            break;
        }
    }

    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(VK_Physical_Device, &physicalDeviceProperties);
    LOG_INFO("Choose physcial device : %s", physicalDeviceProperties.deviceName);
    return true;
}

bool isDeviceSuitable(VkPhysicalDevice device){
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    //Check for graphics operation support and anisotropic filtering support for now
    bool isSuitable = deviceFeatures.samplerAnisotropy;

    if(isSuitable){
        isSuitable = checkQueueFamilySupport(device);
    }
    return true;
}

bool checkQueueFamilySupport(VkPhysicalDevice device){
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

    bool graphicsfamilyFound = false;

    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    for(uint32_t i=0; i<queueFamilyCount; i++){
        if(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            graphicsfamilyFound = true;
            break;
        }
    }

    return graphicsfamilyFound;
}

VkInstance getVulkanInstance(){
    return VK_Instance;
}


void cleanupVulkan(){
    LOG_DEBUG("Destroyed Vk instance");
    vkDestroyInstance(VK_Instance, NULL);
}