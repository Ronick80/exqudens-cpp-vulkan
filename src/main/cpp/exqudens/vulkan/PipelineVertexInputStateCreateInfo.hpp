#pragma once

#include <vector>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct PipelineVertexInputStateCreateInfo: vk::PipelineVertexInputStateCreateInfo {

    std::vector<vk::VertexInputBindingDescription> vertexBindingDescriptions;
    std::vector<vk::VertexInputAttributeDescription> vertexAttributeDescriptions;

    PipelineVertexInputStateCreateInfo& setVertexBindingDescriptions(const std::vector<vk::VertexInputBindingDescription>& value) {
      vertexBindingDescriptions = value;
      vk::PipelineVertexInputStateCreateInfo::setVertexBindingDescriptions(vertexBindingDescriptions);
      return *this;
    }

    PipelineVertexInputStateCreateInfo& setVertexAttributeDescriptions(const std::vector<vk::VertexInputAttributeDescription>& value) {
      vertexAttributeDescriptions = value;
      vk::PipelineVertexInputStateCreateInfo::setVertexAttributeDescriptions(vertexAttributeDescriptions);
      return *this;
    }

  };

}
