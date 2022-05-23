#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct Image {

    unsigned int id;
    vk::ImageCreateInfo createInfo;
    std::shared_ptr<vk::raii::Image> value;
    vk::DeviceSize size;
    std::shared_ptr<vk::raii::DeviceMemory> memory;

  };

}
