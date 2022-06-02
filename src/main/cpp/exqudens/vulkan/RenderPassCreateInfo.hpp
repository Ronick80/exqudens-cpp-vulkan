#pragma once

#include <vector>

#include <vulkan/vulkan_structs.hpp>

#include "exqudens/vulkan/SubPassDescription.hpp"

namespace exqudens::vulkan {

  struct RenderPassCreateInfo {

    std::vector<vk::AttachmentDescription> attachments;
    std::vector<SubPassDescription> subPasses;
    std::vector<vk::SubpassDependency> dependencies;

    RenderPassCreateInfo& setAttachments(const std::vector<vk::AttachmentDescription>& values) {
      attachments = values;
      return *this;
    }

    RenderPassCreateInfo& setSubPasses(const std::vector<SubPassDescription>& values) {
      subPasses = values;
      return *this;
    }

    RenderPassCreateInfo& setDependencies(const std::vector<vk::SubpassDependency>& values) {
      dependencies = values;
      return *this;
    }

  };

}
