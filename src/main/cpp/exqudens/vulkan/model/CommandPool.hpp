#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct CommandPool {

    VkDevice device;
    VkCommandPool value;

  };

}
