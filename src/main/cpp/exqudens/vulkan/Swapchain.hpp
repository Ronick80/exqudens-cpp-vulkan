#pragma once

#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"
#include "exqudens/vulkan/SwapchainCreateInfoKHR.hpp"

namespace exqudens::vulkan {

  struct Swapchain {

    class Builder;

    static Builder builder();

    SwapchainCreateInfoKHR createInfo;
    std::shared_ptr<vk::raii::SwapchainKHR> value;

    vk::raii::SwapchainKHR& reference() {
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

  class Swapchain::Builder {

    private:

      std::weak_ptr<vk::raii::Device> device;
      std::optional<SwapchainCreateInfoKHR> createInfo;

    public:

      Swapchain::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      Swapchain::Builder& setCreateInfo(const SwapchainCreateInfoKHR& val) {
        createInfo = val;
        return *this;
      }

      Swapchain build() {
        try {
          Swapchain target = {};
          target.createInfo = createInfo.value();
          target.value = std::make_shared<vk::raii::SwapchainKHR>(
              *device.lock(),
              target.createInfo
          );
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  Swapchain::Builder Swapchain::builder() {
    return {};
  }

}
