#include "exqudens/vulkan/Functions.hpp"

namespace exqudens::vulkan {

  Configuration Functions::createConfiguration(
      const std::vector<std::string>& extensions
  ) {
    Configuration configuration;
    configuration.extensions = StringVector(extensions);
    return configuration;
  }
}
