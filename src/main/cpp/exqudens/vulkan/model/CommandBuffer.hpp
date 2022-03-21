#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct CommandBuffer {

    VkDevice device;
    VkCommandPool commandPool;
    VkCommandBuffer value;

  };

}
