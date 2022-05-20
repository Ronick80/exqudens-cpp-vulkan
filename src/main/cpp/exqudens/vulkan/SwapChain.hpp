#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct SwapChain {

    unsigned int id;
    vk::SwapchainCreateInfoKHR createInfo;
    std::shared_ptr<vk::raii::SwapchainKHR> value;

  };

}
