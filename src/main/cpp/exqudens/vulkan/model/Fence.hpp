#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Fence {

    VkDevice device;
    VkFence value;

  };

}
