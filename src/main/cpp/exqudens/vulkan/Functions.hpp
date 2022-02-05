#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <ostream>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"
#include "exqudens/vulkan/Logger.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Functions {

    public:

      static Configuration createConfiguration();
      static Configuration createConfiguration(const bool& validationLayersEnabled);
      static Configuration createConfiguration(
          const bool& validationLayersEnabled,
          const std::vector<std::string>&validationLayers,
          const std::vector<std::string>& extensions,
          const std::vector<std::string>& deviceExtensions
      );

      static std::map<std::string, std::string> createEnvironmentVariables(const std::string& executableDirPath);

      static Logger createLogger();
      static Logger createLogger(std::ostream& stream);
      static Logger createLogger(
          const std::function<void(
              VkDebugUtilsMessageSeverityFlagBitsEXT,
              VkDebugUtilsMessageTypeFlagsEXT,
              std::string
          )>& logFunction
      );

      static VkInstance createInstance(Configuration& configuration, Logger& logger);

      static bool checkValidationLayerSupport(const std::vector<std::string>& validationLayers);
      static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& object, Logger& logger);

  };

}
