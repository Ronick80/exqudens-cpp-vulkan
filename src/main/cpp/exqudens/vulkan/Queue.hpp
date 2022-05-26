#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct Queue: Resource<vk::raii::Queue> {

    uint32_t familyIndex;
    uint32_t index;

  };

}
