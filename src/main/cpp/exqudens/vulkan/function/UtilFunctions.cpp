#include "exqudens/vulkan/function/UtilFunctions.hpp"
#include "exqudens/vulkan/Macros.hpp"

#include <set>
#include <cstdlib>
#include <cstdint>
#include <stdexcept>

namespace exqudens::vulkan {

  UtilFunctions::UtilFunctions() = default;

  void UtilFunctions::setEnvironmentVariable(const std::string& name, const std::string& value) {
    try {
#ifdef _WIN32
      _putenv_s(name.c_str(), value.c_str());
#elif _WIN64
      _putenv_s(name.c_str(), value.c_str());
#endif
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  std::optional<std::string> UtilFunctions::getEnvironmentVariable(const std::string& name) {
    try {
      std::optional<std::string> value;
#ifdef _WIN32
      char* buffer;
      size_t size;
      errno_t error = _dupenv_s(&buffer, &size, name.c_str());
      if (error) {
        return value;
      }
      if (buffer != nullptr) {
        value.emplace(std::string(buffer));
      }
#elif _WIN64
      char* buffer;
    size_t size;
    errno_t error = _dupenv_s(&buffer, &size, name.c_str());
    if (error) {
      return value;
    }
    if (buffer != nullptr) {
      value.emplace(std::string(buffer));
    }
#endif
      return value;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  bool UtilFunctions::checkValidationLayerSupport(const std::vector<std::string>& validationLayers) {
    try {
      uint32_t layerCount;
      vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

      std::vector<VkLayerProperties> availableLayers(layerCount);
      vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

      for (const std::string& layerName: validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties: availableLayers) {
          if (layerName == std::string(layerProperties.layerName)) {
            layerFound = true;
            break;
          }
        }

        if (!layerFound) {
          return false;
        }
      }

      return true;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void UtilFunctions::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& object, Logger& logger) {
    try {
      object = {};
      object.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      object.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      object.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      object.pfnUserCallback = &Logger::call;
      object.pUserData = &logger;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  bool UtilFunctions::isDeviceSuitable(
      VkPhysicalDevice& physicalDevice,
      Configuration& configuration,
      VkSurfaceKHR& surface
  ) {
    try {
      if (configuration.presentQueueFamilyRequired && surface == nullptr) {
        throw std::runtime_error(
            CALL_INFO()
            + ": "
            + TO_STRING_SINGLE_QUOTES(configuration.presentQueueFamilyRequired)
            + ": true but "
            + TO_STRING_SINGLE_QUOTES(surface)
            + " is null!"
        );
      }

      QueueFamilyIndices familyIndices = findQueueFamilies(
          physicalDevice,
          configuration.computeQueueFamilyRequired,
          configuration.transferQueueFamilyRequired,
          configuration.graphicsQueueFamilyRequired,
          surface
      );

      bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice, configuration.deviceExtensions);

      bool swapChainAdequate = surface == nullptr;
      if (extensionsSupported && surface != nullptr) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
      }

      VkPhysicalDeviceFeatures supportedFeatures;
      vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

      bool anisotropyAdequate = true;
      if (configuration.anisotropyRequired) {
        anisotropyAdequate = supportedFeatures.samplerAnisotropy;
      }

      return familyIndices.isComplete() &&
          extensionsSupported &&
          swapChainAdequate &&
          anisotropyAdequate;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  QueueFamilyIndices UtilFunctions::findQueueFamilies(
      VkPhysicalDevice& physicalDevice,
      bool computeFamilyRequired,
      bool transferFamilyRequired,
      bool graphicsFamilyRequired,
      VkSurfaceKHR& surface
  ) {
    try {
      QueueFamilyIndices familyIndices;
      familyIndices.computeFamilyRequired = computeFamilyRequired;
      familyIndices.transferFamilyRequired = transferFamilyRequired;
      familyIndices.graphicsFamilyRequired = graphicsFamilyRequired;
      familyIndices.presentFamilyRequired = surface != nullptr;

      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

      int i = 0;
      for (const auto& queueFamily : queueFamilies) {
        if (familyIndices.computeFamilyRequired) {
          if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            familyIndices.computeFamily = i;
          }
        }

        if (familyIndices.transferFamilyRequired) {
          if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
            familyIndices.transferFamily = i;
          }
        }

        if (familyIndices.graphicsFamilyRequired) {
          if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            familyIndices.graphicsFamily = i;
          }
        }

        if (familyIndices.presentFamilyRequired) {
          VkBool32 presentSupport = false;
          vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
          if (presentSupport) {
            familyIndices.presentFamily = i;
          }
        }

        if (familyIndices.isComplete()) {
          break;
        }

        i++;
      }

      return familyIndices;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  bool UtilFunctions::checkDeviceExtensionSupport(VkPhysicalDevice& physicalDevice, StringVector& deviceExtensions) {
    try {
      uint32_t extensionCount;
      vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

      std::vector<VkExtensionProperties> availableExtensions(extensionCount);
      vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

      std::set<std::string> requiredExtensions(deviceExtensions.pointers.begin(), deviceExtensions.pointers.end());

      for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
      }

      return requiredExtensions.empty();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  SwapChainSupportDetails UtilFunctions::querySwapChainSupport(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) {
    try {
      SwapChainSupportDetails details;

      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

      uint32_t formatCount;
      vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

      if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
      }

      uint32_t presentModeCount;
      vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

      if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
      }

      return details;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  UtilFunctions::~UtilFunctions() = default;

}
