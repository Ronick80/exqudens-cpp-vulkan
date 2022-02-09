#pragma once

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT DestroyFunctions {

    public:

      DestroyFunctions();

      virtual void destroyPhysicalDevice(VkPhysicalDevice& physicalDevice);

      virtual void destroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT& debugUtilsMessenger, VkInstance& instance);

      virtual void destroyInstance(VkInstance& instance);

      virtual ~DestroyFunctions();

  };

}
