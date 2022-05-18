#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Queue.hpp"

namespace exqudens::vulkan {

  struct PhysicalDevice {

    unsigned int id;
    std::vector<Queue> computeQueues;
    std::vector<Queue> transferQueues;
    std::vector<Queue> graphicsQueues;
    std::vector<Queue> presentQueues;
    vk::PhysicalDeviceFeatures features;
    vk::raii::PhysicalDevice* value;

  };

}
