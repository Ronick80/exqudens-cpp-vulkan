#pragma once

#include <cstdint>
#include <vector>
#include <map>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct PhysicalDevice {

    unsigned int id;
    float queuePriorities;
    std::vector<vk::DeviceQueueCreateInfo> computeQueueCreateInfos;
    std::vector<vk::DeviceQueueCreateInfo> transferQueueCreateInfos;
    std::vector<vk::DeviceQueueCreateInfo> graphicsQueueCreateInfos;
    std::vector<vk::DeviceQueueCreateInfo> presentQueueCreateInfos;
    std::vector<vk::DeviceQueueCreateInfo> uniqueQueueCreateInfos;
    vk::PhysicalDeviceFeatures features;
    vk::raii::PhysicalDevice* value;

  };

}
