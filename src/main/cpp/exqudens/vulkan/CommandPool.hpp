#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct CommandPool: Resource<vk::raii::CommandPool> {

    vk::CommandPoolCreateInfo createInfo;

  };

}
