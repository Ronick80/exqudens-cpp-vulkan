#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT SwapChain {

    public:

      VkFormat format = VkFormat::VK_FORMAT_UNDEFINED;
      VkExtent2D extent = {};
      uint32_t imageCount = 0;
      uint32_t width = 0;
      uint32_t height = 0;
      VkSwapchainKHR value = nullptr;

      SwapChain(
          VkFormat format,
          VkExtent2D extent,
          uint32_t imageCount,
          uint32_t width,
          uint32_t height,
          VkSwapchainKHR value
      );
      SwapChain();
      SwapChain(const SwapChain& object);
      SwapChain(SwapChain&& object) noexcept;

      SwapChain& operator=(const SwapChain& object);
      SwapChain& operator=(SwapChain&& object) noexcept;

      friend void swap(SwapChain& first, SwapChain& second);

      virtual ~SwapChain();

  };

}
