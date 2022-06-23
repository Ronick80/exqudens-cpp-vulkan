#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct PipelineViewportStateCreateInfo: vk::PipelineViewportStateCreateInfo {

    PipelineViewportStateCreateInfo& setFlags(const vk::PipelineViewportStateCreateFlags& value) {
      vk::PipelineViewportStateCreateInfo::setFlags(value);
      return *this;
    }

    std::vector<vk::Viewport> viewports;
    std::vector<vk::Rect2D> scissors;

    PipelineViewportStateCreateInfo& setViewports(const std::vector<vk::Viewport>& value) {
      viewports = value;
      vk::PipelineViewportStateCreateInfo::setViewports(viewports);
      return *this;
    }

    PipelineViewportStateCreateInfo& setScissors(const std::vector<vk::Rect2D>& value) {
      scissors = value;
      vk::PipelineViewportStateCreateInfo::setScissors(scissors);
      return *this;
    }

  };

}
