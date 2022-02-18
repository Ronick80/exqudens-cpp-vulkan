#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Buffer {

    VkDeviceMemory memory;
    VkBuffer value;

  };

}
