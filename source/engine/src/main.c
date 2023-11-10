#include "../includes/utils/logger.h"
#include "../includes/vulkan/vulkan_core.h"

int main(){
    LOG_INFO("Starting application...");

    initializeVulkan();

    LOG_INFO("Application stopped");
    return 0;
}