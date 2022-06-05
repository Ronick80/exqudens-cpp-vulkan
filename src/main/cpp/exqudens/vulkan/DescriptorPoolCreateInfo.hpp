#pragma once

#include <vector>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct DescriptorPoolCreateInfo: vk::DescriptorPoolCreateInfo {

    DescriptorPoolCreateInfo& setFlags(const vk::DescriptorPoolCreateFlags& value) {
      vk::DescriptorPoolCreateInfo::setFlags(value);
      return *this;
    }

    DescriptorPoolCreateInfo& setMaxSets(const uint32_t& value) {
      vk::DescriptorPoolCreateInfo::setMaxSets(value);
      return *this;
    }

    std::vector<vk::DescriptorPoolSize> poolSizes;

    DescriptorPoolCreateInfo& setPoolSizes(const std::vector<vk::DescriptorPoolSize>& value) {
      poolSizes = value;
      vk::DescriptorPoolCreateInfo::setPoolSizes(poolSizes);
      return *this;
    }

  };

}
