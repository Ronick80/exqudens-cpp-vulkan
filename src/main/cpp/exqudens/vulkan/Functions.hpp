#pragma once

#include <string>
#include <vector>

#include "exqudens/vulkan/Export.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Functions {

    public:

      static Configuration createConfiguration(
          const std::vector<std::string>& extensions
      );

  };

}
