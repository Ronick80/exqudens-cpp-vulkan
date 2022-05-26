#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct Surface: Resource<vk::raii::SurfaceKHR> {

  };

}
