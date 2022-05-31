#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct Fence: Resource<vk::raii::Fence> {

    vk::FenceCreateInfo createInfo;

  };

}
