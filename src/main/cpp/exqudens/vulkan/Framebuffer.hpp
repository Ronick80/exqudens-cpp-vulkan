#pragma once

#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"
#include "exqudens/vulkan/FramebufferCreateInfo.hpp"

namespace exqudens::vulkan {

  struct Framebuffer {

    class Builder;

    static Builder builder();

    FramebufferCreateInfo createInfo;
    std::shared_ptr<vk::raii::Framebuffer> value;

    vk::raii::Framebuffer& reference() {
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

  class Framebuffer::Builder {

    private:

      std::weak_ptr<vk::raii::Device> device;
      std::optional<FramebufferCreateInfo> createInfo;

    public:

      Framebuffer::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      Framebuffer::Builder& setCreateInfo(const FramebufferCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      Framebuffer build() {
        try {
          Framebuffer target = {};
          target.createInfo = createInfo.value();
          target.value = std::make_shared<vk::raii::Framebuffer>(
              *device.lock(),
              target.createInfo
          );
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  Framebuffer::Builder Framebuffer::builder() {
    return {};
  }

}
