#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Queue {

    uint32_t familyIndex;
    VkQueue value;

  };

}
