#pragma once

#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/model/Device.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"

namespace exqudens::vulkan {

  class DeviceFactory : virtual public Utility {

    public:

      virtual Device createDevice(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          QueueFamilyIndexInfo& queueFamilyIndexInfo
      ) = 0;

      virtual void destroyDevice(Device& device) = 0;

  };

}
