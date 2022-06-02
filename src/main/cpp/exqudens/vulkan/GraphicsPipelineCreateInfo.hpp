#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct GraphicsPipelineCreateInfo {

    vk::PipelineCreateFlags flags;
    std::vector<vk::PipelineShaderStageCreateInfo> stages;
    std::optional<vk::PipelineVertexInputStateCreateInfo> vertexInputState;
    std::optional<vk::PipelineInputAssemblyStateCreateInfo> inputAssemblyState;
    std::optional<vk::PipelineTessellationStateCreateInfo> tessellationState;
    std::optional<vk::PipelineViewportStateCreateInfo> viewportState;
    std::optional<vk::PipelineRasterizationStateCreateInfo> rasterizationState;
    std::optional<vk::PipelineMultisampleStateCreateInfo> multisampleState;
    std::optional<vk::PipelineDepthStencilStateCreateInfo> depthStencilState;
    std::optional<vk::PipelineColorBlendStateCreateInfo> colorBlendState;
    std::optional<vk::PipelineDynamicStateCreateInfo> dynamicState;
    vk::PipelineLayout layout;
    vk::RenderPass renderPass;
    uint32_t subpass;
    vk::Pipeline basePipelineHandle;
    int32_t basePipelineIndex;

    GraphicsPipelineCreateInfo& setFlags(const vk::PipelineCreateFlags& value) {
      flags = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setStages(const std::vector<vk::PipelineShaderStageCreateInfo>& values) {
      stages = values;
      return *this;
    }

    GraphicsPipelineCreateInfo& setVertexInputState(const vk::PipelineVertexInputStateCreateInfo& value) {
      vertexInputState = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setInputAssemblyState(const vk::PipelineInputAssemblyStateCreateInfo& value) {
      inputAssemblyState = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setTessellationState(const vk::PipelineTessellationStateCreateInfo& value) {
      tessellationState = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setViewportState(const vk::PipelineViewportStateCreateInfo& value) {
      viewportState = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setRasterizationState(const vk::PipelineRasterizationStateCreateInfo& value) {
      rasterizationState = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setMultisampleState(const vk::PipelineMultisampleStateCreateInfo& value) {
      multisampleState = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setDepthStencilState(const vk::PipelineDepthStencilStateCreateInfo& value) {
      depthStencilState = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setColorBlendState(const vk::PipelineColorBlendStateCreateInfo& value) {
      colorBlendState = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setDynamicState(const vk::PipelineDynamicStateCreateInfo& value) {
      dynamicState = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setLayout(const vk::PipelineLayout& value) {
      layout = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setRenderPass(const vk::RenderPass& value) {
      renderPass = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setSubpass(uint32_t value) {
      subpass = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setBasePipelineHandle(const vk::Pipeline& value) {
      basePipelineHandle = value;
      return *this;
    }

    GraphicsPipelineCreateInfo& setBasePipelineIndex(int32_t value) {
      basePipelineIndex = value;
      return *this;
    }

  };

}
