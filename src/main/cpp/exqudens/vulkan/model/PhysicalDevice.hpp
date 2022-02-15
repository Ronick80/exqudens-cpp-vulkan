#pragma once

#include <optional>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/model/QueueFamilyIndexInfo.hpp"
#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"

namespace exqudens::vulkan {

  class PhysicalDevice {

    public:

      QueueFamilyIndexInfo queueFamilyIndexInfo;
      std::optional<SwapChainSupportDetails> swapChainSupportDetails;
      VkPhysicalDevice value = nullptr;

      PhysicalDevice(
          QueueFamilyIndexInfo queueFamilyIndexInfo,
          std::optional<SwapChainSupportDetails> swapChainSupportDetails,
          VkPhysicalDevice value
      );
      PhysicalDevice();
      PhysicalDevice(const PhysicalDevice& object);
      PhysicalDevice(PhysicalDevice&& object) noexcept;

      PhysicalDevice& operator=(const PhysicalDevice& object);
      PhysicalDevice& operator=(PhysicalDevice&& object) noexcept;

      friend void swap(PhysicalDevice& first, PhysicalDevice& second);

      virtual ~PhysicalDevice();

  };

}
