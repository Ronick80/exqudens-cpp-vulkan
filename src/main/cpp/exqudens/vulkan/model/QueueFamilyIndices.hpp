#pragma once

#include <optional>
#include <cstdint>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT QueueFamilyIndices {

    public:

      std::optional<uint32_t> transferFamily;
      std::optional<uint32_t> graphicsFamily;
      std::optional<uint32_t> presentFamily;

      QueueFamilyIndices(
          std::optional<uint32_t> transferFamily,
          std::optional<uint32_t> graphicsFamily,
          std::optional<uint32_t> presentFamily
      );
      QueueFamilyIndices();
      QueueFamilyIndices(const QueueFamilyIndices& object);
      QueueFamilyIndices(QueueFamilyIndices&& object) noexcept;

      QueueFamilyIndices& operator=(const QueueFamilyIndices& object);
      QueueFamilyIndices& operator=(QueueFamilyIndices&& object) noexcept;

      friend void swap(QueueFamilyIndices& first, QueueFamilyIndices& second);

      bool isComplete();

      virtual ~QueueFamilyIndices();

  };

}
