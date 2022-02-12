#pragma once

#include <string>
#include <vector>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Configuration {

    public:

      std::string applicationName = {};
      unsigned int applicationVersionMajor = 0;
      unsigned int applicationVersionMinor = 0;
      unsigned int applicationVersionPatch = 0;
      std::string engineName = {};
      unsigned int engineVersionMajor = 0;
      unsigned int engineVersionMinor = 0;
      unsigned int engineVersionPatch = 0;
      bool validationLayersEnabled = false;
      std::vector<const char*> validationLayers = {};
      std::vector<const char*> extensions = {};
      std::vector<const char*> deviceExtensions = {};
      bool computeQueueFamilyRequired = true;
      bool transferQueueFamilyRequired = true;
      bool graphicsQueueFamilyRequired = true;
      bool presentQueueFamilyRequired = true;
      bool anisotropyRequired = true;

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
          std::vector<const char*> validationLayers,
          std::vector<const char*> extensions,
          std::vector<const char*> deviceExtensions,
          bool computeQueueFamilyRequired,
          bool transferQueueFamilyRequired,
          bool graphicsQueueFamilyRequired,
          bool presentQueueFamilyRequired,
          bool anisotropyRequired
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
