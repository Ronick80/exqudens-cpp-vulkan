#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <ostream>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"
#include "exqudens/vulkan/Logger.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"
#include "exqudens/vulkan/function/UtilFunctions.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT CreateFunctions {

    public:

      UtilFunctions* utilFunctions = nullptr;

      explicit CreateFunctions(UtilFunctions* utilFunctions);
      CreateFunctions();

      virtual std::map<std::string, std::string> createEnvironmentVariables(const std::string& executableDirPath);

      virtual Configuration createConfiguration();
      virtual Configuration createConfiguration(const bool& validationLayersEnabled);
      virtual Configuration createConfiguration(
          std::string applicationName,
          unsigned int applicationVersionMajor,
          unsigned int applicationVersionMinor,
          unsigned int applicationVersionPatch,
          std::string engineName,
          unsigned int engineVersionMajor,
          unsigned int engineVersionMinor,
          unsigned int engineVersionPatch,
          const bool& validationLayersEnabled,
          const std::vector<std::string>& validationLayers,
          const std::vector<std::string>& extensions,
          const std::vector<std::string>& deviceExtensions
      );

      virtual Logger createLogger();
      virtual Logger createLogger(std::ostream& stream);
      virtual Logger createLogger(
          const std::function<void(
              VkDebugUtilsMessageSeverityFlagBitsEXT,
              VkDebugUtilsMessageTypeFlagsEXT,
              const std::string&
          )>& function
      );

      virtual VkInstance createInstance(Configuration& configuration, Logger& logger);

      virtual VkDebugUtilsMessengerEXT createDebugUtilsMessenger(VkInstance& instance, Logger& logger);

      virtual VkPhysicalDevice createPhysicalDevice(
          VkInstance& instance,
          VkSurfaceKHR& surface,
          StringVector& deviceExtensions,
          bool transferFamilyRequired
      );

      virtual ~CreateFunctions();

  };

}
