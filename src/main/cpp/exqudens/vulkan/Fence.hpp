#pragma once

#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan {

  struct Fence {

    class Builder;

    static Builder builder();

    vk::FenceCreateInfo createInfo;
    std::shared_ptr<vk::raii::Fence> value;

    vk::raii::Fence& reference() {
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

  class Fence::Builder {

    private:

      std::weak_ptr<vk::raii::Device> device;
      std::optional<vk::FenceCreateInfo> createInfo;

    public:

      Fence::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      Fence::Builder& setCreateInfo(const vk::FenceCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      Fence build() {
        try {
          Fence target = {};
          target.createInfo = createInfo.value_or(vk::FenceCreateInfo());
          target.value = std::make_shared<vk::raii::Fence>(
              *device.lock(),
              target.createInfo
          );
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  Fence::Builder Fence::builder() {
    return {};
  }

}
