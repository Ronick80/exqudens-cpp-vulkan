#pragma once

#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan {

  struct DescriptorSetLayout {

    class Builder;

    static Builder builder();

    std::vector<vk::DescriptorSetLayoutBinding> bindings;
    vk::DescriptorSetLayoutCreateInfo createInfo;
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
      std::vector<vk::DescriptorSetLayoutBinding> bindings;
      std::optional<vk::DescriptorSetLayoutCreateInfo> createInfo;

    public:

      DescriptorSetLayout::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      DescriptorSetLayout::Builder& addBinding(const vk::DescriptorSetLayoutBinding& val) {
        bindings.emplace_back(val);
        return *this;
      }

      DescriptorSetLayout::Builder& setBindings(const std::vector<vk::DescriptorSetLayoutBinding>& val) {
        bindings = val;
        return *this;
      }

      DescriptorSetLayout::Builder& setCreateInfo(const vk::DescriptorSetLayoutCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      DescriptorSetLayout build() {
        try {
          DescriptorSetLayout target = {};
          target.bindings = bindings;
          target.createInfo = createInfo.value_or(vk::DescriptorSetLayoutCreateInfo());
          target.createInfo.setBindings(target.bindings);
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
