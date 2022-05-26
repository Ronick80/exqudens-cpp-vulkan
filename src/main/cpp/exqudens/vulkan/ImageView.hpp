#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct ImageView: Resource<vk::raii::ImageView> {

    vk::ImageViewCreateInfo createInfo;

  };

}
