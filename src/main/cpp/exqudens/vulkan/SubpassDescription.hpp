#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct SubpassDescription: vk::SubpassDescription {

    std::vector<vk::AttachmentReference> inputAttachments;
    std::vector<vk::AttachmentReference> colorAttachments;
    std::vector<vk::AttachmentReference> resolveAttachments;
    std::optional<vk::AttachmentReference> depthStencilAttachment;
    std::vector<uint32_t> preserveAttachments;

    SubpassDescription& setInputAttachments(const std::vector<vk::AttachmentReference>& values) {
      inputAttachments = values;
      vk::SubpassDescription::setInputAttachments(inputAttachments);
      return *this;
    }

    SubpassDescription& setColorAttachments(const std::vector<vk::AttachmentReference>& values) {
      colorAttachments = values;
      vk::SubpassDescription::setColorAttachments(colorAttachments);
      return *this;
    }

    SubpassDescription& setResolveAttachments(const std::vector<vk::AttachmentReference>& values) {
      resolveAttachments = values;
      vk::SubpassDescription::setResolveAttachments(resolveAttachments);
      return *this;
    }

    SubpassDescription& setDepthStencilAttachment(const vk::AttachmentReference& value) {
      depthStencilAttachment = value;
      vk::SubpassDescription::setPDepthStencilAttachment(&depthStencilAttachment.value());
      return *this;
    }

    SubpassDescription& setPreserveAttachments(const std::vector<uint32_t>& values) {
      preserveAttachments = values;
      vk::SubpassDescription::setPreserveAttachments(preserveAttachments);
      return *this;
    }

  };

}
