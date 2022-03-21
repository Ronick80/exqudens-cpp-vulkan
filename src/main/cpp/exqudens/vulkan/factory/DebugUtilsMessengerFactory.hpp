#pragma once

#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/Logger.hpp"
#include "exqudens/vulkan/model/DebugUtilsMessenger.hpp"

namespace exqudens::vulkan {

  class DebugUtilsMessengerFactory : virtual public Utility {

    public:

      virtual DebugUtilsMessenger createDebugUtilsMessenger(VkInstance& instance, Logger& logger) = 0;

      virtual void destroyDebugUtilsMessenger(DebugUtilsMessenger& debugUtilsMessenger) = 0;

  };

}
