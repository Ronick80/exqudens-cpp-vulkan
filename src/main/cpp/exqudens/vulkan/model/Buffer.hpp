#pragma once

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Buffer {

    public:

      VkBuffer value = nullptr;
      VkDeviceMemory memory = nullptr;

      Buffer(
          VkBuffer value,
          VkDeviceMemory memory
      );
      Buffer();
      Buffer(const Buffer& object);
      Buffer(Buffer&& object) noexcept;

      virtual Buffer& operator=(const Buffer& object);
      virtual Buffer& operator=(Buffer&& object) noexcept;

      friend void swap(Buffer& first, Buffer& second);

      virtual ~Buffer();

  };

}
