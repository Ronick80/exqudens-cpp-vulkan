#pragma once

#include <vector>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct DescriptorSetAllocateInfo: vk::DescriptorSetAllocateInfo {

    DescriptorSetAllocateInfo& setDescriptorPool(const vk::DescriptorPool& value) {
      vk::DescriptorSetAllocateInfo::setDescriptorPool(value);
      return *this;
    }

    DescriptorSetAllocateInfo& setDescriptorSetCount(const uint32_t& value) {
      vk::DescriptorSetAllocateInfo::setDescriptorSetCount(1);
      return *this;
    }

    std::vector<vk::DescriptorSetLayout> setLayouts;

    DescriptorSetAllocateInfo& setSetLayouts(const std::vector<vk::DescriptorSetLayout>& value) {
      setLayouts = value;
      vk::DescriptorSetAllocateInfo::setSetLayouts(setLayouts);
      return *this;
    }

  };

}
