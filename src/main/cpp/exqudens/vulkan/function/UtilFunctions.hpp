#pragma once

#include <string>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"
#include "exqudens/vulkan/Logger.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"
#include "exqudens/vulkan/model/StringVector.hpp"
#include "exqudens/vulkan/model/QueueFamilyIndices.hpp"
#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT UtilFunctions {

    public:

      UtilFunctions();

      virtual void setEnvironmentVariable(const std::string& name, const std::string& value);

      virtual std::optional<std::string> getEnvironmentVariable(const std::string& name);

      virtual bool checkValidationLayerSupport(const std::vector<std::string>& validationLayers);

      virtual void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& object, Logger& logger);

      virtual bool isDeviceSuitable(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface
      );

      virtual QueueFamilyIndices findQueueFamilies(
          VkPhysicalDevice& physicalDevice,
          bool computeFamilyRequired,
          bool transferFamilyRequired,
          bool graphicsFamilyRequired,
          VkSurfaceKHR& surface
      );

      virtual bool checkDeviceExtensionSupport(VkPhysicalDevice& physicalDevice, StringVector& deviceExtensions);

      virtual SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);

      virtual ~UtilFunctions();

  };

}
