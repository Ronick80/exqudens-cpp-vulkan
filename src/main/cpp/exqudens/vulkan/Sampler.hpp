#pragma once

#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan {

  struct Sampler {

    class Builder;

    static Builder builder();

    vk::SamplerCreateInfo createInfo;
    std::shared_ptr<vk::raii::Sampler> value;

    vk::raii::Sampler& reference() {
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

  class Sampler::Builder {

    private:

      std::weak_ptr<vk::raii::Device> device;
      std::optional<vk::SamplerCreateInfo> createInfo;

    public:

      Sampler::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      Sampler::Builder& setCreateInfo(const vk::SamplerCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      Sampler build() {
        try {
          Sampler target = {};
          target.createInfo = createInfo.value();
          target.value = std::make_shared<vk::raii::Sampler>(
              *device.lock(),
              target.createInfo
          );
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  Sampler::Builder Sampler::builder() {
    return {};
  }

}
