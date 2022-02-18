#pragma once

#include <optional>
#include <cstdint>
#include <set>

namespace exqudens::vulkan {

  struct QueueFamilyIndexInfo {

    bool computeFamilyRequired;
    std::optional<uint32_t> computeFamily;

    bool transferFamilyRequired;
    std::optional<uint32_t> transferFamily;

    bool graphicsFamilyRequired;
    std::optional<uint32_t> graphicsFamily;

    bool presentFamilyRequired;
    std::optional<uint32_t> presentFamily;

    bool complete;
    std::set<uint32_t> uniqueQueueFamilyIndices;

  };

}
