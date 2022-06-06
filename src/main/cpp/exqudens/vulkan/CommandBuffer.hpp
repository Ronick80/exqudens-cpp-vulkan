#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct CommandBuffer: Resource<vk::raii::CommandBuffer> {

    vk::CommandBufferAllocateInfo createInfo;

  };

}
