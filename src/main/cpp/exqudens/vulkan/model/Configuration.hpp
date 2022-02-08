#pragma once

#include <utility>

#include "exqudens/vulkan/Export.hpp"
#include "exqudens/vulkan/model/StringVector.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Configuration {

    public:

      bool validationLayersEnabled = false;
      StringVector validationLayers = StringVector();
      StringVector extensions = StringVector();
      StringVector deviceExtensions = StringVector();

      Configuration(
          bool validationLayersEnabled,
          StringVector validationLayers,
          StringVector extensions,
          StringVector deviceExtensions
      );
      Configuration();
      Configuration(const Configuration& object);
      Configuration(Configuration&& object) noexcept;

      Configuration& operator=(const Configuration& object);
      Configuration& operator=(Configuration&& object) noexcept;

      friend void swap(Configuration& first, Configuration& second);

      virtual ~Configuration();

  };

}
