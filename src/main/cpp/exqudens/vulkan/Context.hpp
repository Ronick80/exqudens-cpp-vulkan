#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"
#include "exqudens/vulkan/ContextCreateInfo.hpp"

namespace exqudens::vulkan {

  struct Context: Resource<vk::raii::Context> {

    ContextCreateInfo createInfo;

  };

}
