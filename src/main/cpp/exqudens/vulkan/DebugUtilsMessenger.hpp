#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct DebugUtilsMessenger {

    unsigned int id;
    vk::DebugUtilsMessengerCreateInfoEXT createInfo;
    std::shared_ptr<vk::raii::DebugUtilsMessengerEXT> value;

  };

}
