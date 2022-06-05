#pragma once

#include <vector>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct PipelineLayoutCreateInfo: vk::PipelineLayoutCreateInfo {

    PipelineLayoutCreateInfo& setFlags(const vk::PipelineLayoutCreateFlags& value) {
      vk::PipelineLayoutCreateInfo::setFlags(value);
      return *this;
    }

    std::vector<vk::DescriptorSetLayout> setLayouts;
    std::vector<vk::PushConstantRange> pushConstantRanges;

    PipelineLayoutCreateInfo& setSetLayouts(const std::vector<vk::DescriptorSetLayout>& value) {
      setLayouts = value;
      vk::PipelineLayoutCreateInfo::setSetLayouts(setLayouts);
      return *this;
    }

    PipelineLayoutCreateInfo& setPushConstantRanges(const std::vector<vk::PushConstantRange>& value) {
      pushConstantRanges = value;
      vk::PipelineLayoutCreateInfo::setPushConstantRanges(pushConstantRanges);
      return *this;
    }

  };

}
