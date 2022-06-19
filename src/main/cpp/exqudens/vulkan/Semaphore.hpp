#pragma once

#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan {

  struct Semaphore {

    class Builder;

    static Builder builder();

    vk::SemaphoreCreateInfo createInfo;
    std::shared_ptr<vk::raii::Semaphore> value;

    vk::raii::Semaphore& reference() {
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

  class Semaphore::Builder {

    private:

      std::weak_ptr<vk::raii::Device> device;
      std::optional<vk::SemaphoreCreateInfo> createInfo;

    public:

      Semaphore::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      Semaphore::Builder& setCreateInfo(const vk::SemaphoreCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      Semaphore build() {
        try {
          Semaphore target = {};
          target.createInfo = createInfo.value_or(vk::SemaphoreCreateInfo());
          target.value = std::make_shared<vk::raii::Semaphore>(
              *device.lock(),
              target.createInfo
          );
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  Semaphore::Builder Semaphore::builder() {
    return {};
  }

}
