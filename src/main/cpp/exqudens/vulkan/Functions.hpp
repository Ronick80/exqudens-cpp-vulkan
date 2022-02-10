#pragma once

#include <string>
#include <optional>
#include <vector>
#include <map>
#include <functional>
#include <ostream>
#include <cstdint>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"
#include "exqudens/vulkan/Logger.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"
#include "exqudens/vulkan/model/StringVector.hpp"
#include "exqudens/vulkan/model/QueueType.hpp"
#include "exqudens/vulkan/model/QueueFamilyIndices.hpp"
#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Functions {

    public:

      std::function<VkSurfaceKHR(VkInstance&)> createSurfaceFunction;

      explicit Functions(
          std::function<VkSurfaceKHR(VkInstance&)> createSurfaceFunction
      );
      Functions();
      Functions(const Functions& object);
      Functions(Functions&& object) noexcept;

      Functions& operator=(const Functions& object);
      Functions& operator=(Functions&& object) noexcept;

      friend void swap(Functions& first, Functions& second);

      // create

      virtual std::map<std::string, std::string> createEnvironmentVariables(const std::string& executableDirPath);

      virtual Configuration createConfiguration();

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

      virtual VkSurfaceKHR createSurface(VkInstance& instance);

      virtual VkPhysicalDevice createPhysicalDevice(
          VkInstance& instance,
          Configuration& configuration,
          VkSurfaceKHR& surface
      );

      virtual VkDevice createDevice(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface
      );

      virtual VkQueue createQueue(
          QueueType type,
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface,
          VkDevice& device,
          uint32_t queueIndex
      );

      // utility

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

      // destroy

      virtual void destroyDevice(VkDevice& device);

      virtual void destroyPhysicalDevice(VkPhysicalDevice& physicalDevice);

      virtual void destroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT& debugUtilsMessenger, VkInstance& instance);

      virtual void destroyInstance(VkInstance& instance);

      virtual ~Functions();

  };

}
