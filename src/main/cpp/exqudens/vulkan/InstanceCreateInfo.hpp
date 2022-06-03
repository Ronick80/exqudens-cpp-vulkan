#pragma once

#include <optional>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct InstanceCreateInfo: vk::InstanceCreateInfo {

    std::optional<vk::ApplicationInfo> applicationInfo;
    std::vector<const char*> enabledExtensionNames;
    std::vector<const char*> enabledLayerNames;

    InstanceCreateInfo& setApplicationInfo(const vk::ApplicationInfo& value) {
      applicationInfo = value;
      setPApplicationInfo(&applicationInfo.value());
      return *this;
    }

    InstanceCreateInfo& setEnabledExtensionNames(const std::vector<const char*>& value) {
      enabledExtensionNames = value;
      setPEnabledExtensionNames(enabledExtensionNames);
      return *this;
    }

    InstanceCreateInfo& setEnabledLayerNames(const std::vector<const char*>& value) {
      enabledLayerNames = value;
      setPEnabledLayerNames(enabledLayerNames);
      return *this;
    }

  };

}
