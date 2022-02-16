#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Queue {

    public:

      uint32_t familyIndex;
      VkQueue value = nullptr;

      Queue(
          uint32_t familyIndex,
          VkQueue value
      );
      Queue();
      Queue(const Queue& object);
      Queue(Queue&& object) noexcept;

      Queue& operator=(const Queue& object);
      Queue& operator=(Queue&& object) noexcept;

      friend void swap(Queue& first, Queue& second);

      virtual ~Queue();

  };

}
