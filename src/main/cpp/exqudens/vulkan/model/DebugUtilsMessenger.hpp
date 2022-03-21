#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct DebugUtilsMessenger {

    VkInstance instance;
    VkDebugUtilsMessengerEXT value;

  };

}
