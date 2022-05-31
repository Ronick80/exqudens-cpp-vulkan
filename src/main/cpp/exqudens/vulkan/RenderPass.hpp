#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"
#include "exqudens/vulkan/RenderPassCreateInfo.hpp"

namespace exqudens::vulkan {

  struct RenderPass: Resource<vk::raii::RenderPass> {

    RenderPassCreateInfo createInfo;

  };

}
