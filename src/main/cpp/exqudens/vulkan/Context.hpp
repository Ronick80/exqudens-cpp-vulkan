#pragma once

#include "exqudens/vulkan/Factory.hpp"

namespace exqudens::vulkan {

  class Context : virtual public Factory {

    public:

      virtual void destroy() = 0;

      virtual ~Context() = default;

  };

}
