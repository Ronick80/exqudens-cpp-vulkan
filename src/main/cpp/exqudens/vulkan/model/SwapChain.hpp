#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  class SwapChain {

    public:

      VkFormat format = VkFormat::VK_FORMAT_UNDEFINED;
      VkExtent2D extent = {};
      int width = 0;
      int height = 0;
      VkSwapchainKHR value = nullptr;

      SwapChain(
          VkFormat format,
          VkExtent2D extent,
          int width,
          int height,
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
