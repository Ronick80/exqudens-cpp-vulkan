#pragma once

#include <optional>
#include <cstdint>
#include <set>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT QueueFamilyIndexInfo {

    public:

      bool computeFamilyRequired = true;
      std::optional<uint32_t> computeFamily = {};

      bool transferFamilyRequired = true;
      std::optional<uint32_t> transferFamily = {};

      bool graphicsFamilyRequired = true;
      std::optional<uint32_t> graphicsFamily = {};

      bool presentFamilyRequired = true;
      std::optional<uint32_t> presentFamily = {};

      bool complete = false;
      std::set<uint32_t> uniqueQueueFamilyIndices = {};

      QueueFamilyIndexInfo(
          bool computeFamilyRequired,
          std::optional<uint32_t> computeFamily,

          bool transferFamilyRequired,
          std::optional<uint32_t> transferFamily,

          bool graphicsFamilyRequired,
          std::optional<uint32_t> graphicsFamily,

          bool presentFamilyRequired,
          std::optional<uint32_t> presentFamily,

          bool complete,
          std::set<uint32_t> uniqueQueueFamilyIndices
      );
      QueueFamilyIndexInfo();
      QueueFamilyIndexInfo(const QueueFamilyIndexInfo& object);
      QueueFamilyIndexInfo(QueueFamilyIndexInfo&& object) noexcept;

      QueueFamilyIndexInfo& operator=(const QueueFamilyIndexInfo& object);
      QueueFamilyIndexInfo& operator=(QueueFamilyIndexInfo&& object) noexcept;

      friend void swap(QueueFamilyIndexInfo& first, QueueFamilyIndexInfo& second);

      virtual ~QueueFamilyIndexInfo();

  };

}
