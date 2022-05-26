#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct Instance: Resource<vk::raii::Instance> {

    vk::ApplicationInfo applicationInfo;
    vk::InstanceCreateInfo createInfo;

  };

}
