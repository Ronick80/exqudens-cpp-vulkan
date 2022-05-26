#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct DebugUtilsMessenger: Resource<vk::raii::DebugUtilsMessengerEXT> {

    vk::DebugUtilsMessengerCreateInfoEXT createInfo;

  };

}
