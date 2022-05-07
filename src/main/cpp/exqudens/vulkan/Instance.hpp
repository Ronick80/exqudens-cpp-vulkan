#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct Instance {

    unsigned int id;
    vk::ApplicationInfo applicationInfo;
    vk::InstanceCreateInfo createInfo;
    std::shared_ptr<vk::raii::Instance> value;

  };

}
