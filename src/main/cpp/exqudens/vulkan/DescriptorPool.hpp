#pragma once

#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan {

  struct DescriptorPool {

    class Builder;

    static Builder builder();

    std::vector<vk::DescriptorPoolSize> poolSizes;
    vk::DescriptorPoolCreateInfo createInfo;
    std::shared_ptr<vk::raii::DescriptorPool> value;

    vk::raii::DescriptorPool& reference() {
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

  class DescriptorPool::Builder {

    private:

      std::weak_ptr<vk::raii::Device> device;
      std::vector<vk::DescriptorPoolSize> poolSizes;
      std::optional<vk::DescriptorPoolCreateInfo> createInfo;

    public:

      DescriptorPool::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      DescriptorPool::Builder& addPoolSize(const vk::DescriptorPoolSize& val) {
        poolSizes.emplace_back(val);
        return *this;
      }

      DescriptorPool::Builder& setPoolSizes(const std::vector<vk::DescriptorPoolSize>& val) {
        poolSizes = val;
        return *this;
      }

      DescriptorPool::Builder& setCreateInfo(const vk::DescriptorPoolCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      DescriptorPool build() {
        try {
          DescriptorPool target = {};
          target.poolSizes = poolSizes;
          target.createInfo = createInfo.value();
          target.createInfo.setPoolSizes(target.poolSizes);
          //target.createInfo.setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet);
          target.value = std::make_shared<vk::raii::DescriptorPool>(
              *device.lock(),
              target.createInfo
          );
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  DescriptorPool::Builder DescriptorPool::builder() {
    return {};
  }

}
