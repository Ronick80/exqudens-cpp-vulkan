#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct Semaphore: Resource<vk::raii::Semaphore> {

    vk::SemaphoreCreateInfo createInfo;

  };

}
