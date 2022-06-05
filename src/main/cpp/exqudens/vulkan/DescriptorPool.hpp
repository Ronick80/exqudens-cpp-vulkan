#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/DescriptorPoolCreateInfo.hpp"
#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct DescriptorPool: Resource<vk::raii::DescriptorPool> {

    DescriptorPoolCreateInfo createInfo;

  };

}
