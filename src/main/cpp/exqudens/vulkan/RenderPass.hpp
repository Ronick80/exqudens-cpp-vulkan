#pragma once

#include <optional>
#include <vector>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"
#include "exqudens/vulkan/RenderPassCreateInfo.hpp"

namespace exqudens::vulkan {

  struct RenderPass {

    class Builder;

    static Builder builder();

    RenderPassCreateInfo createInfo;
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

      std::optional<RenderPassCreateInfo> createInfo;
      std::weak_ptr<vk::raii::Device> device;

    public:

      RenderPass::Builder& setCreateInfo(const RenderPassCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      RenderPass::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      RenderPass build() {
        try {
          RenderPass target = {};
          target.createInfo = createInfo.value();
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
