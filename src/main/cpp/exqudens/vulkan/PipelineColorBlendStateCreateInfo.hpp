#pragma once

#include <array>
#include <vector>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct PipelineColorBlendStateCreateInfo: vk::PipelineColorBlendStateCreateInfo {

    PipelineColorBlendStateCreateInfo& setFlags(const vk::PipelineColorBlendStateCreateFlags& value) {
      vk::PipelineColorBlendStateCreateInfo::setFlags(value);
      return *this;
    }

    PipelineColorBlendStateCreateInfo& setLogicOpEnable(const bool& value) {
      vk::PipelineColorBlendStateCreateInfo::setLogicOpEnable(value);
      return *this;
    }

    PipelineColorBlendStateCreateInfo& setLogicOp(const vk::LogicOp& value) {
      vk::PipelineColorBlendStateCreateInfo::setLogicOp(value);
      return *this;
    }

    PipelineColorBlendStateCreateInfo& setBlendConstants(const std::array<float, 4>& value) {
      vk::PipelineColorBlendStateCreateInfo::setBlendConstants(value);
      return *this;
    }

    std::vector<vk::PipelineColorBlendAttachmentState> attachments;

    PipelineColorBlendStateCreateInfo& setAttachments(const std::vector<vk::PipelineColorBlendAttachmentState>& value) {
      attachments = value;
      vk::PipelineColorBlendStateCreateInfo::setAttachments(attachments);
      return *this;
    }

  };

}
