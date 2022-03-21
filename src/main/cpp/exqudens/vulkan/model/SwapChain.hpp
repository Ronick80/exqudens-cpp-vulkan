#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct SwapChain {

    VkDevice device;
    VkFormat format;
    VkExtent2D extent;
    uint32_t imageCount;
    uint32_t width;
    uint32_t height;
    VkSwapchainKHR value;

  };

}
