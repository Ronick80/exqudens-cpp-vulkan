#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct Device: Resource<vk::raii::Device> {

    vk::DeviceCreateInfo createInfo;

  };

}
