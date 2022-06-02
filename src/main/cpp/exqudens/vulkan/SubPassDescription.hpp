#pragma once

#include <vector>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct SubPassDescription {

    vk::SubpassDescriptionFlags flags;
    vk::PipelineBindPoint pipelineBindPoint;
    std::vector<vk::AttachmentReference> inputAttachments;
    std::vector<vk::AttachmentReference> colorAttachments;
    std::vector<vk::AttachmentReference> resolveAttachments;
    vk::AttachmentReference depthStencilAttachment;
    std::vector<uint32_t> preserveAttachments;

    SubPassDescription& setFlags(const vk::SubpassDescriptionFlags& value) {
      flags = value;
      return *this;
    }

    SubPassDescription& setPipelineBindPoint(const vk::PipelineBindPoint& value) {
      pipelineBindPoint = value;
      return *this;
    }

    SubPassDescription& setInputAttachments(const std::vector<vk::AttachmentReference>& values) {
      inputAttachments = values;
      return *this;
    }

    SubPassDescription& setColorAttachments(const std::vector<vk::AttachmentReference>& values) {
      colorAttachments = values;
      return *this;
    }

    SubPassDescription& setResolveAttachments(const std::vector<vk::AttachmentReference>& values) {
      resolveAttachments = values;
      return *this;
    }

    SubPassDescription& setDepthStencilAttachment(const vk::AttachmentReference& value) {
      depthStencilAttachment = value;
      return *this;
    }

    SubPassDescription& setPreserveAttachments(const std::vector<uint32_t>& values) {
      preserveAttachments = values;
      return *this;
    }

  };

}
