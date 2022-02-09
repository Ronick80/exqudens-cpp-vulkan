#pragma once

#include <optional>
#include <cstdint>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT QueueFamilyIndices {

    public:

      bool computeFamilyRequired = true;
      std::optional<uint32_t> computeFamily;

      bool transferFamilyRequired = true;
      std::optional<uint32_t> transferFamily;

      bool graphicsFamilyRequired = true;
      std::optional<uint32_t> graphicsFamily;

      bool presentFamilyRequired = true;
      std::optional<uint32_t> presentFamily;

      QueueFamilyIndices(
          bool computeFamilyRequired,
          std::optional<uint32_t> computeFamily,

          bool transferFamilyRequired,
          std::optional<uint32_t> transferFamily,

          bool graphicsFamilyRequired,
          std::optional<uint32_t> graphicsFamily,

          bool presentFamilyRequired,
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
