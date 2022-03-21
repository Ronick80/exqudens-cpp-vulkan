#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Semaphore {

    VkDevice device;
    VkSemaphore value;

  };

}
