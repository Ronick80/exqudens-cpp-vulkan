#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  class SwapChain {

    public:

      VkFormat format = VkFormat::VK_FORMAT_UNDEFINED;
      VkExtent2D extent = {};
      VkSwapchainKHR value = nullptr;

      SwapChain(
          VkFormat format,
          VkExtent2D extent,
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
