#pragma once

#include <string>
#include <optional>
#include <vector>
#include <map>
#include <functional>
#include <ostream>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"
#include "exqudens/vulkan/Logger.hpp"
#include "exqudens/vulkan/model/QueueFamilyIndices.hpp"
#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Functions {

    public:

      static std::map<std::string, std::string> createEnvironmentVariables(const std::string& executableDirPath);

      static Configuration createConfiguration();
      static Configuration createConfiguration(const bool& validationLayersEnabled);
      static Configuration createConfiguration(
          std::string applicationName,
          unsigned int applicationVersionMajor,
          unsigned int applicationVersionMinor,
          unsigned int applicationVersionPatch,
          std::string engineName,
          unsigned int engineVersionMajor,
          unsigned int engineVersionMinor,
          unsigned int engineVersionPatch,
          const bool& validationLayersEnabled,
          const std::vector<std::string>&validationLayers,
          const std::vector<std::string>& extensions,
          const std::vector<std::string>& deviceExtensions
      );

      static Logger createLogger();
      static Logger createLogger(std::ostream& stream);
      static Logger createLogger(
          const std::function<void(
              VkDebugUtilsMessageSeverityFlagBitsEXT,
              VkDebugUtilsMessageTypeFlagsEXT,
              const std::string&
          )>& function
      );

      static VkInstance createInstance(Configuration& configuration, Logger& logger);
      static VkDebugUtilsMessengerEXT createDebugUtilsMessenger(Logger& logger, VkInstance& instance);
      //static VkSurfaceKHR createSurface(GLFWwindow*& window, VkInstance& instance);
      static VkPhysicalDevice createPhysicalDevice(VkInstance& instance, VkSurfaceKHR& surface, StringVector& deviceExtensions);

      static void setEnvironmentVariable(const std::string& name, const std::string& value);
      static std::optional<std::string> getEnvironmentVariable(const std::string& name);
      static bool checkValidationLayerSupport(const std::vector<std::string>& validationLayers);
      static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& object, Logger& logger);
      static bool isDeviceSuitable(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, StringVector& deviceExtensions);
      static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);
      static bool checkDeviceExtensionSupport(VkPhysicalDevice& physicalDevice, StringVector& deviceExtensions);
      static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);

      static void destroyDebugUtilsMessenger(VkInstance& instance, VkDebugUtilsMessengerEXT& debugUtilsMessenger);
      static void destroyInstance(VkInstance& instance);

  };

}
