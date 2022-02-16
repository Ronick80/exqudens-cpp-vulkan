#pragma once

//#include <cstdint>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Image {

    public:

      //VkImageTiling tiling = VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
      //VkImageUsageFlags usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
      //VkMemoryPropertyFlags properties = VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
      uint32_t width = 0;
      uint32_t height = 0;
      VkFormat format = VkFormat::VK_FORMAT_UNDEFINED; // VkFormat::VK_FORMAT_R8G8B8A8_SRGB || VK_FORMAT_R8G8B8A8_UNORM
      VkDeviceMemory memory = nullptr;
      VkImage value = nullptr;

      Image(
          uint32_t width,
          uint32_t height,
          VkFormat format,
          VkDeviceMemory memory,
          VkImage value
      );
      Image();
      Image(const Image& object);
      Image(Image&& object) noexcept;

      Image& operator=(const Image& object);
      Image& operator=(Image&& object) noexcept;

      friend void swap(Image& first, Image& second);

      virtual ~Image();

  };

}
