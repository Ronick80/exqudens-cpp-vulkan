#pragma once

#include "exqudens/vulkan/Logger.hpp"
#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/model/Instance.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"

namespace exqudens::vulkan {

  class InstanceFactory : virtual public Utility {

    public:

      virtual Instance createInstance(
          Configuration& configuration,
          Logger& logger
      ) = 0;

      virtual void destroyInstance(
          Instance& instance
      ) = 0;

  };

}
