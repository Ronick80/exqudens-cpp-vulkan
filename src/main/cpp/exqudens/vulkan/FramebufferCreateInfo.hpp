#pragma once

#include <vector>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct FramebufferCreateInfo: vk::FramebufferCreateInfo {

    FramebufferCreateInfo& setFlags(const vk::FramebufferCreateFlags& value) {
      vk::FramebufferCreateInfo::setFlags(value);
      return *this;
    }

    FramebufferCreateInfo& setRenderPass(const vk::RenderPass& value) {
      vk::FramebufferCreateInfo::setRenderPass(value);
      return *this;
    }

    FramebufferCreateInfo& setLayers(const uint32_t& value) {
      vk::FramebufferCreateInfo::setLayers(value);
      return *this;
    }

    FramebufferCreateInfo& setWidth(const uint32_t& value) {
      vk::FramebufferCreateInfo::setWidth(value);
      return *this;
    }

    FramebufferCreateInfo& setHeight(const uint32_t& value) {
      vk::FramebufferCreateInfo::setHeight(value);
      return *this;
    }

    std::vector<vk::ImageView> attachments;

    FramebufferCreateInfo& setAttachments(const std::vector<vk::ImageView>& value) {
      attachments = value;
      vk::FramebufferCreateInfo::setAttachments(attachments);
      return *this;
    }

  };

}
