#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Buffer {

    VkBuffer value;
    VkDeviceMemory memory;

  };

}
