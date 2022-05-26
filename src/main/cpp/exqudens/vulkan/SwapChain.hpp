#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct SwapChain: Resource<vk::raii::SwapchainKHR> {

    vk::SwapchainCreateInfoKHR createInfo;

  };

}
