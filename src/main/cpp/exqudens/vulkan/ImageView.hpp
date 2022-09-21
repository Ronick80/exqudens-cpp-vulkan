#pragma once

#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan {

  struct ImageView {

    class Builder;

    inline static Builder builder();

    vk::ImageViewCreateInfo createInfo;
    std::shared_ptr<vk::raii::ImageView> value;

    vk::raii::ImageView& reference() {
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

  class ImageView::Builder {

    private:

      std::weak_ptr<vk::raii::Device> device;
      std::optional<vk::ImageViewCreateInfo> createInfo;

    public:

      ImageView::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      ImageView::Builder& setCreateInfo(const vk::ImageViewCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      ImageView build() {
        try {
          ImageView target = {};
          target.createInfo = createInfo.value();
          target.value = std::make_shared<vk::raii::ImageView>(
              *device.lock(),
              target.createInfo
          );
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  ImageView::Builder ImageView::builder() {
    return {};
  }

}
