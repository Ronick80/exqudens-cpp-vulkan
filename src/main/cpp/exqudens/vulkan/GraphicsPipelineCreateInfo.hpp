#pragma once

#include <cstdint>
#include <optional>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/PipelineVertexInputStateCreateInfo.hpp"
#include "exqudens/vulkan/PipelineViewportStateCreateInfo.hpp"
#include "exqudens/vulkan/PipelineColorBlendStateCreateInfo.hpp"

namespace exqudens::vulkan {

  struct GraphicsPipelineCreateInfo: vk::GraphicsPipelineCreateInfo {

    GraphicsPipelineCreateInfo& setFlags(const vk::PipelineCreateFlags& value) {
      vk::GraphicsPipelineCreateInfo::setFlags(value);
      return *this;
    }

    GraphicsPipelineCreateInfo& setRenderPass(const vk::RenderPass& value) {
      vk::GraphicsPipelineCreateInfo::setRenderPass(value);
      return *this;
    }

    GraphicsPipelineCreateInfo& setSubpass(const uint32_t& value) {
      vk::GraphicsPipelineCreateInfo::setSubpass(value);
      return *this;
    }

    GraphicsPipelineCreateInfo& setBasePipelineIndex(const int32_t& value) {
      vk::GraphicsPipelineCreateInfo::setBasePipelineIndex(value);
      return *this;
    }

    GraphicsPipelineCreateInfo& setBasePipelineHandle(const vk::Pipeline& value) {
      vk::GraphicsPipelineCreateInfo::setBasePipelineHandle(value);
      return *this;
    }

    std::optional<PipelineVertexInputStateCreateInfo> vertexInputState;
    std::optional<vk::PipelineInputAssemblyStateCreateInfo> inputAssemblyState;
    std::optional<vk::PipelineTessellationStateCreateInfo> tessellationState;
    std::optional<PipelineViewportStateCreateInfo> viewportState;
    std::optional<vk::PipelineRasterizationStateCreateInfo> rasterizationState;
    std::optional<vk::PipelineMultisampleStateCreateInfo> multisampleState;
    std::optional<vk::PipelineDepthStencilStateCreateInfo> depthStencilState;
    std::optional<PipelineColorBlendStateCreateInfo> colorBlendState;
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

    GraphicsPipelineCreateInfo& setViewportState(const PipelineViewportStateCreateInfo& value) {
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

    GraphicsPipelineCreateInfo& setColorBlendState(const PipelineColorBlendStateCreateInfo& value) {
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
