#pragma once

#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"
#include "exqudens/vulkan/model/PhysicalDevice.hpp"

namespace exqudens::vulkan {

  class PhysicalDeviceFactory: virtual public Utility {

    public:

      virtual PhysicalDevice createPhysicalDevice(
          VkInstance& instance,
          Configuration& configuration
      ) = 0;

      virtual PhysicalDevice createPhysicalDevice(
          VkInstance& instance,
          Configuration& configuration,
          const VkSurfaceKHR& surface
      ) = 0;

      virtual void destroyPhysicalDevice(PhysicalDevice& physicalDevice) = 0;

  };

}
