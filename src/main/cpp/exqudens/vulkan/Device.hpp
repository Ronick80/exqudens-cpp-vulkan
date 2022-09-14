#pragma once

#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan {

  struct Device {

    class Builder;

    inline static Builder builder();

    vk::DeviceCreateInfo createInfo;
    std::shared_ptr<vk::raii::Device> value;

    vk::raii::Device& reference() {
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

  class Device::Builder {

    private:

      std::weak_ptr<vk::raii::PhysicalDevice> physicalDevice;
      std::optional<vk::DeviceCreateInfo> createInfo;

    public:

      Device::Builder& setPhysicalDevice(const std::weak_ptr<vk::raii::PhysicalDevice>& val) {
        physicalDevice = val;
        return *this;
      }

      Device::Builder& setCreateInfo(const vk::DeviceCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      Device build() {
        try {
          Device target = {};
          target.createInfo = createInfo.value();
          target.value = std::make_shared<vk::raii::Device>(
              *physicalDevice.lock(),
              target.createInfo
          );
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  Device::Builder Device::builder() {
    return {};
  }

}
