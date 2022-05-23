#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct ImageView {

    unsigned int id;
    vk::ImageViewCreateInfo createInfo;
    std::shared_ptr<vk::raii::ImageView> value;

  };

}
