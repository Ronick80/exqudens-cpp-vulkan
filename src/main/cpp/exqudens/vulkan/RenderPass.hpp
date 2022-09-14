#pragma once

#include <optional>
#include <vector>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"
#include "exqudens/vulkan/SubpassDescription.hpp"

namespace exqudens::vulkan {

  struct RenderPass {

    class Builder;

    inline static Builder builder();

    std::vector<vk::AttachmentDescription> attachments;
    std::vector<SubpassDescription> subpasses;
    std::vector<vk::SubpassDependency> dependencies;
    vk::RenderPassCreateInfo createInfo;
    std::shared_ptr<vk::raii::RenderPass> value;

    vk::raii::RenderPass& reference() {
      try {
        if (!value) {
          throw std::runtime_error(CALL_INFO() + ": value is not initialized!");
        }
        return *value;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

  };

  class RenderPass::Builder {

    private:

      std::weak_ptr<vk::raii::Device> device;
      std::vector<vk::AttachmentDescription> attachments;
      std::vector<SubpassDescription> subpasses;
      std::vector<vk::SubpassDependency> dependencies;
      std::optional<vk::RenderPassCreateInfo> createInfo;

    public:

      RenderPass::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      RenderPass::Builder& addAttachment(const vk::AttachmentDescription& val) {
        attachments.emplace_back(val);
        return *this;
      }

      RenderPass::Builder& setAttachments(const std::vector<vk::AttachmentDescription>& val) {
        attachments = val;
        return *this;
      }

      RenderPass::Builder& addSubpass(const SubpassDescription& val) {
        subpasses.emplace_back(val);
        return *this;
      }

      RenderPass::Builder& setSubpasses(const std::vector<SubpassDescription>& val) {
        subpasses = val;
        return *this;
      }

      RenderPass::Builder& addDependency(const vk::SubpassDependency& val) {
        dependencies.emplace_back(val);
        return *this;
      }

      RenderPass::Builder& setDependencies(const std::vector<vk::SubpassDependency>& val) {
        dependencies = val;
        return *this;
      }

      RenderPass::Builder& setCreateInfo(const vk::RenderPassCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      RenderPass build() {
        try {
          RenderPass target = {};
          target.attachments = attachments;
          target.subpasses = subpasses;
          target.dependencies = dependencies;
          target.createInfo = createInfo.value_or(vk::RenderPassCreateInfo());
          target.createInfo.setAttachments(target.attachments);
          target.createInfo.setSubpassCount(target.subpasses.size());
          target.createInfo.setPSubpasses(target.subpasses.data());
          target.createInfo.setDependencies(target.dependencies);
          target.value = std::make_shared<vk::raii::RenderPass>(
              *device.lock(),
              target.createInfo
          );
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  RenderPass::Builder RenderPass::builder() {
    return {};
  }

}
