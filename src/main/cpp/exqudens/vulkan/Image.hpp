#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/MemoryResource.hpp"

namespace exqudens::vulkan {

  struct Image: MemoryResource<vk::raii::Image, vk::raii::DeviceMemory> {

    vk::ImageCreateInfo createInfo;

  };

}
