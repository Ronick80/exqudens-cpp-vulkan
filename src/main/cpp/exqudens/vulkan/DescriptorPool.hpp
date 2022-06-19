#pragma once

#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"
#include "exqudens/vulkan/DescriptorPoolCreateInfo.hpp"
#include "exqudens/vulkan/DescriptorSetAllocateInfo.hpp"

namespace exqudens::vulkan {

  struct DescriptorPool {

    class Builder;

    static Builder builder();

    DescriptorPoolCreateInfo createInfo;
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
      std::optional<DescriptorPoolCreateInfo> createInfo;

    public:

      DescriptorPool::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      DescriptorPool::Builder& setCreateInfo(const DescriptorPoolCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      DescriptorPool build() {
        try {
          DescriptorPool target = {};
          target.createInfo = createInfo.value();
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
