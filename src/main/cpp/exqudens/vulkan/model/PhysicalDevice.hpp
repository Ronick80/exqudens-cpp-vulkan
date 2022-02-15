#pragma once

#include <optional>
#include <set>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/model/QueueFamilyIndices.hpp"
#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"

namespace exqudens::vulkan {

  class PhysicalDevice {

    public:

      QueueFamilyIndices queueFamilyIndicesInfo;
      std::optional<SwapChainSupportDetails> swapChainSupportDetails;
      std::set<uint32_t> queueFamilyIndices = {};
      VkPhysicalDevice value = nullptr;

      PhysicalDevice(
          QueueFamilyIndices queueFamilyIndices,
          std::optional<SwapChainSupportDetails> swapChainSupportDetails,
          std::set<uint32_t> uniqueQueueFamilyIndices,
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
