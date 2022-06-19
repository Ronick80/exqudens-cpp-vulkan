#pragma once

#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"
#include "exqudens/vulkan/DescriptorSetLayoutCreateInfo.hpp"

namespace exqudens::vulkan {

  struct DescriptorSetLayout {

    class Builder;

    static Builder builder();

    DescriptorSetLayoutCreateInfo createInfo;
    std::shared_ptr<vk::raii::DescriptorSetLayout> value;

    vk::raii::DescriptorSetLayout& reference() {
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

  class DescriptorSetLayout::Builder {

    private:

      std::weak_ptr<vk::raii::Device> device;
      std::optional<DescriptorSetLayoutCreateInfo> createInfo;

    public:

      DescriptorSetLayout::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      DescriptorSetLayout::Builder& setCreateInfo(const DescriptorSetLayoutCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      DescriptorSetLayout build() {
        try {
          DescriptorSetLayout target = {};
          target.createInfo = createInfo.value();
          target.value = std::make_shared<vk::raii::DescriptorSetLayout>(
              *device.lock(),
              target.createInfo
          );
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  DescriptorSetLayout::Builder DescriptorSetLayout::builder() {
    return {};
  }

}
