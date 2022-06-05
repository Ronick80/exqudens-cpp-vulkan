#pragma once

#include <vector>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct DescriptorSetLayoutCreateInfo: vk::DescriptorSetLayoutCreateInfo {

    DescriptorSetLayoutCreateInfo& setFlags(const vk::DescriptorSetLayoutCreateFlags& value) {
      vk::DescriptorSetLayoutCreateInfo::setFlags(value);
      return *this;
    }

    std::vector<vk::DescriptorSetLayoutBinding> bindings;

    DescriptorSetLayoutCreateInfo& setBindings(const std::vector<vk::DescriptorSetLayoutBinding>& value) {
      bindings = value;
      vk::DescriptorSetLayoutCreateInfo::setBindings(bindings);
      return *this;
    }

  };

}
