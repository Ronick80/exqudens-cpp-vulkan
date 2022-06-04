#pragma once

#include <vector>

#include <vulkan/vulkan_structs.hpp>

#include "exqudens/vulkan/SubpassDescription.hpp"

namespace exqudens::vulkan {

  struct RenderPassCreateInfo: vk::RenderPassCreateInfo {

    std::vector<vk::AttachmentDescription> attachments;
    std::vector<vk::SubpassDescription> subpasses;
    std::vector<vk::SubpassDependency> dependencies;

    RenderPassCreateInfo& setAttachments(const std::vector<vk::AttachmentDescription>& values) {
      attachments = values;
      vk::RenderPassCreateInfo::setAttachments(attachments);
      return *this;
    }

    RenderPassCreateInfo& setSubpasses(const std::vector<vk::SubpassDescription>& values) {
      subpasses = values;
      vk::RenderPassCreateInfo::setSubpasses(subpasses);
      return *this;
    }

    RenderPassCreateInfo& setDependencies(const std::vector<vk::SubpassDependency>& values) {
      dependencies = values;
      vk::RenderPassCreateInfo::setDependencies(dependencies);
      return *this;
    }

  };

}
