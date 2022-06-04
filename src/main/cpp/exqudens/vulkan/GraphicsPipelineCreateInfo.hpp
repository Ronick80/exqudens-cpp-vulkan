#pragma once

#include <cstdint>
#include <optional>

#include <vulkan/vulkan_structs.hpp>

#include "exqudens/vulkan/PipelineVertexInputStateCreateInfo.hpp"

namespace exqudens::vulkan {

  struct GraphicsPipelineCreateInfo: vk::GraphicsPipelineCreateInfo {

    std::optional<PipelineVertexInputStateCreateInfo> vertexInputState;
    std::optional<vk::PipelineInputAssemblyStateCreateInfo> inputAssemblyState;
    std::optional<vk::PipelineTessellationStateCreateInfo> tessellationState;
    std::optional<vk::PipelineViewportStateCreateInfo> viewportState;
    std::optional<vk::PipelineRasterizationStateCreateInfo> rasterizationState;
    std::optional<vk::PipelineMultisampleStateCreateInfo> multisampleState;
    std::optional<vk::PipelineDepthStencilStateCreateInfo> depthStencilState;
    std::optional<vk::PipelineColorBlendStateCreateInfo> colorBlendState;
    std::optional<vk::PipelineDynamicStateCreateInfo> dynamicState;

    GraphicsPipelineCreateInfo& setVertexInputState(const PipelineVertexInputStateCreateInfo& value) {
      vertexInputState = value;
      vk::GraphicsPipelineCreateInfo::setPVertexInputState(&vertexInputState.value());
      return *this;
    }

    GraphicsPipelineCreateInfo& setInputAssemblyState(const vk::PipelineInputAssemblyStateCreateInfo& value) {
      inputAssemblyState = value;
      vk::GraphicsPipelineCreateInfo::setPInputAssemblyState(&inputAssemblyState.value());
      return *this;
    }

    GraphicsPipelineCreateInfo& setTessellationState(const vk::PipelineTessellationStateCreateInfo& value) {
      tessellationState = value;
      vk::GraphicsPipelineCreateInfo::setPTessellationState(&tessellationState.value());
      return *this;
    }

    GraphicsPipelineCreateInfo& setViewportState(const vk::PipelineViewportStateCreateInfo& value) {
      viewportState = value;
      vk::GraphicsPipelineCreateInfo::setPViewportState(&viewportState.value());
      return *this;
    }

    GraphicsPipelineCreateInfo& setRasterizationState(const vk::PipelineRasterizationStateCreateInfo& value) {
      rasterizationState = value;
      vk::GraphicsPipelineCreateInfo::setPRasterizationState(&rasterizationState.value());
      return *this;
    }

    GraphicsPipelineCreateInfo& setMultisampleState(const vk::PipelineMultisampleStateCreateInfo& value) {
      multisampleState = value;
      vk::GraphicsPipelineCreateInfo::setPMultisampleState(&multisampleState.value());
      return *this;
    }

    GraphicsPipelineCreateInfo& setDepthStencilState(const vk::PipelineDepthStencilStateCreateInfo& value) {
      depthStencilState = value;
      vk::GraphicsPipelineCreateInfo::setPDepthStencilState(&depthStencilState.value());
      return *this;
    }

    GraphicsPipelineCreateInfo& setColorBlendState(const vk::PipelineColorBlendStateCreateInfo& value) {
      colorBlendState = value;
      vk::GraphicsPipelineCreateInfo::setPColorBlendState(&colorBlendState.value());
      return *this;
    }

    GraphicsPipelineCreateInfo& setDynamicState(const vk::PipelineDynamicStateCreateInfo& value) {
      dynamicState = value;
      vk::GraphicsPipelineCreateInfo::setPDynamicState(&dynamicState.value());
      return *this;
    }

  };

}
