#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/SubPassDescription.hpp"

namespace exqudens::vulkan {

  struct RenderPassCreateInfo {

    std::vector<vk::AttachmentDescription> attachments;
    std::vector<SubPassDescription> subPassesHolder;
    std::vector<vk::SubpassDependency> dependencies;
    std::vector<vk::SubpassDescription> subPasses;

    RenderPassCreateInfo& setAttachments(const std::vector<vk::AttachmentDescription>& values) {
      attachments = values;
      return *this;
    }

    RenderPassCreateInfo& setSubPasses(const std::vector<SubPassDescription>& values) {
      subPassesHolder = values;
      subPasses = {};
      for (const SubPassDescription& s : subPassesHolder) {
        subPasses.emplace_back(
            vk::SubpassDescription()
                .setFlags(s.flags)
                .setPipelineBindPoint(s.pipelineBindPoint)
                .setInputAttachments(s.inputAttachments)
                .setColorAttachments(s.colorAttachments)
                .setResolveAttachments(s.resolveAttachments)
                .setPDepthStencilAttachment(&s.depthStencilAttachment)
                .setPreserveAttachments(s.preserveAttachments)
        );
      }
      return *this;
    }

    RenderPassCreateInfo& setDependencies(const std::vector<vk::SubpassDependency>& values) {
      dependencies = values;
      return *this;
    }

  };

}
