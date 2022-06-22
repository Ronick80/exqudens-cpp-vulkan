#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct SubpassDescription: vk::SubpassDescription {

    SubpassDescription& setFlags(const vk::SubpassDescriptionFlags& value) {
      vk::SubpassDescription::setFlags(value);
      return *this;
    }

    SubpassDescription& setPipelineBindPoint(const vk::PipelineBindPoint& value) {
      vk::SubpassDescription::setPipelineBindPoint(value);
      return *this;
    }

    std::vector<vk::AttachmentReference> inputAttachments;
    std::vector<vk::AttachmentReference> colorAttachments;
    std::vector<vk::AttachmentReference> resolveAttachments;
    std::optional<vk::AttachmentReference> depthStencilAttachment;
    std::vector<uint32_t> preserveAttachments;

    SubpassDescription& addInputAttachment(const vk::AttachmentReference& val) {
      inputAttachments.emplace_back(val);
      vk::SubpassDescription::setInputAttachments(inputAttachments);
      return *this;
    }

    SubpassDescription& setInputAttachments(const std::vector<vk::AttachmentReference>& val) {
      inputAttachments = val;
      vk::SubpassDescription::setInputAttachments(inputAttachments);
      return *this;
    }

    SubpassDescription& addColorAttachment(const vk::AttachmentReference& val) {
      colorAttachments.emplace_back(val);
      vk::SubpassDescription::setColorAttachments(colorAttachments);
      return *this;
    }

    SubpassDescription& setColorAttachments(const std::vector<vk::AttachmentReference>& val) {
      colorAttachments = val;
      vk::SubpassDescription::setColorAttachments(colorAttachments);
      return *this;
    }

    SubpassDescription& addResolveAttachment(const vk::AttachmentReference& val) {
      resolveAttachments.emplace_back(val);
      vk::SubpassDescription::setResolveAttachments(resolveAttachments);
      return *this;
    }

    SubpassDescription& setResolveAttachments(const std::vector<vk::AttachmentReference>& val) {
      resolveAttachments = val;
      vk::SubpassDescription::setResolveAttachments(resolveAttachments);
      return *this;
    }

    SubpassDescription& setDepthStencilAttachment(const vk::AttachmentReference& val) {
      depthStencilAttachment = val;
      vk::SubpassDescription::setPDepthStencilAttachment(&depthStencilAttachment.value());
      return *this;
    }

    SubpassDescription& addPreserveAttachment(const uint32_t& val) {
      preserveAttachments.emplace_back(val);
      vk::SubpassDescription::setPreserveAttachments(preserveAttachments);
      return *this;
    }

    SubpassDescription& setPreserveAttachments(const std::vector<uint32_t>& val) {
      preserveAttachments = val;
      vk::SubpassDescription::setPreserveAttachments(preserveAttachments);
      return *this;
    }

  };

}
