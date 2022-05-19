#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct Device {

    unsigned int id;
    vk::DeviceCreateInfo createInfo;
    std::shared_ptr<vk::raii::Device> value;

  };

}
