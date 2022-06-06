#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"
#include "exqudens/vulkan/FramebufferCreateInfo.hpp"

namespace exqudens::vulkan {

  struct Framebuffer: Resource<vk::raii::Framebuffer> {

    FramebufferCreateInfo createInfo;

  };

}
