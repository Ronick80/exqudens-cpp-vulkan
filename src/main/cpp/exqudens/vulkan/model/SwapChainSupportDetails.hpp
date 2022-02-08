#pragma once

#include <vector>

#include "vulkan/vulkan.h"

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT SwapChainSupportDetails {

    public:

      VkSurfaceCapabilitiesKHR capabilities = {};
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;

      SwapChainSupportDetails(
          VkSurfaceCapabilitiesKHR capabilities,
          std::vector<VkSurfaceFormatKHR> formats,
          std::vector<VkPresentModeKHR> presentModes
      );
      SwapChainSupportDetails();
      SwapChainSupportDetails(const SwapChainSupportDetails& object);
      SwapChainSupportDetails(SwapChainSupportDetails&& object) noexcept;

      SwapChainSupportDetails& operator=(const SwapChainSupportDetails& object);
      SwapChainSupportDetails& operator=(SwapChainSupportDetails&& object) noexcept;

      friend void swap(SwapChainSupportDetails& first, SwapChainSupportDetails& second);

      virtual ~SwapChainSupportDetails();

  };

}
