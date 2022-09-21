#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct QueueRequirement {

    vk::QueueFlagBits type;
    bool timestampRequired;

    QueueRequirement& setType(const vk::QueueFlagBits& val) {
      type = val;
      return *this;
    }

    QueueRequirement& setTimestampRequired(const bool& val) {
      timestampRequired = val;
      return *this;
    }

  };

}
