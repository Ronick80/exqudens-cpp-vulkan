#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/MemoryResource.hpp"

namespace exqudens::vulkan {

  struct Buffer: MemoryResource<vk::raii::Buffer, vk::raii::DeviceMemory> {

    vk::BufferCreateInfo createInfo;

  };

}
