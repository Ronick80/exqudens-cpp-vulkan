#pragma once

#include <string>
#include <vector>

#include "exqudens/vulkan/Export.hpp"
#include "exqudens/vulkan/model/StringVector.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Configuration {

    public:

      std::string applicationName;
      unsigned int applicationVersionMajor = 0;
      unsigned int applicationVersionMinor = 0;
      unsigned int applicationVersionPatch = 0;
      std::string engineName;
      unsigned int engineVersionMajor = 0;
      unsigned int engineVersionMinor = 0;
      unsigned int engineVersionPatch = 0;
      bool validationLayersEnabled = false;
      StringVector validationLayers = StringVector();
      StringVector extensions = StringVector();
      StringVector deviceExtensions = StringVector();

      Configuration(
          std::string applicationName,
          unsigned int applicationVersionMajor,
          unsigned int applicationVersionMinor,
          unsigned int applicationVersionPatch,
          std::string engineName,
          unsigned int engineVersionMajor,
          unsigned int engineVersionMinor,
          unsigned int engineVersionPatch,
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
