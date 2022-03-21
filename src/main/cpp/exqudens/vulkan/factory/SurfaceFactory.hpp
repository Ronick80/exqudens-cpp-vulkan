#pragma once

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/model/Surface.hpp"

namespace exqudens::vulkan {

  class SurfaceFactory : virtual Utility {

    public:

      virtual Surface createSurface(VkInstance& instance) = 0;

      virtual void destroySurface(Surface& surface) = 0;

  };

}
