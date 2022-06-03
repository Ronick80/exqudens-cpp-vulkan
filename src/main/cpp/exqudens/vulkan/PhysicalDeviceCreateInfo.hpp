#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct PhysicalDeviceCreateInfo {

    std::vector<const char*> enabledExtensionNames;
    vk::PhysicalDeviceFeatures features;
    std::vector<vk::QueueFlagBits> queueTypes;
    std::optional<vk::SurfaceKHR> surface;
    float queuePriorities;

    PhysicalDeviceCreateInfo& setEnabledExtensionNames(const std::vector<const char*>& value) {
      enabledExtensionNames = value;
      return *this;
    }

    PhysicalDeviceCreateInfo& setFeatures(const vk::PhysicalDeviceFeatures& value) {
      features = value;
      return *this;
    }

    PhysicalDeviceCreateInfo& setQueueTypes(const std::vector<vk::QueueFlagBits>& value) {
      queueTypes = value;
      return *this;
    }

    PhysicalDeviceCreateInfo& setSurface(const vk::SurfaceKHR& value) {
      surface = value;
      return *this;
    }

    PhysicalDeviceCreateInfo& setQueuePriorities(const float& value) {
      queuePriorities = value;
      return *this;
    }

  };

}
