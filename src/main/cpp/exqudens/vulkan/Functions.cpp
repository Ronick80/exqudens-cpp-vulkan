#include "exqudens/vulkan/Functions.hpp"
#include "exqudens/vulkan/Macros.hpp"

#include <set>
#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <iostream>

namespace exqudens::vulkan {

  std::map<std::string, std::string> Functions::createEnvironmentVariables(const std::string& executableDirPath) {
    try {
      std::map<std::string, std::string> environmentVariables;
      environmentVariables["VK_LAYER_PATH"] = std::filesystem::path(executableDirPath).make_preferred().string();
      return environmentVariables;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Configuration Functions::createConfiguration() {
    try {
      std::string applicationName = "Exqudens Application";
      unsigned int applicationVersionMajor = 1;
      unsigned int applicationVersionMinor = 0;
      unsigned int applicationVersionPatch = 0;
      std::string engineName = "Engine Application";
      unsigned int engineVersionMajor = 1;
      unsigned int engineVersionMinor = 0;
      unsigned int engineVersionPatch = 0;
      bool validationLayersEnabled = true;
      std::vector<std::string> validationLayers = {"VK_LAYER_KHRONOS_validation"};
      std::vector<std::string> extensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
      std::vector<std::string> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
      return createConfiguration(
          applicationName,
          applicationVersionMajor,
          applicationVersionMinor,
          applicationVersionPatch,
          engineName,
          engineVersionMajor,
          engineVersionMinor,
          engineVersionPatch,
          validationLayersEnabled,
          validationLayers,
          extensions,
          deviceExtensions
      );
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Configuration Functions::createConfiguration(const bool& validationLayersEnabled) {
    try {
      std::string applicationName = "Exqudens Application";
      unsigned int applicationVersionMajor = 1;
      unsigned int applicationVersionMinor = 0;
      unsigned int applicationVersionPatch = 0;
      std::string engineName = "Engine Application";
      unsigned int engineVersionMajor = 1;
      unsigned int engineVersionMinor = 0;
      unsigned int engineVersionPatch = 0;
      std::vector<std::string> validationLayers;
      if (validationLayersEnabled) {
        validationLayers = {"VK_LAYER_KHRONOS_validation"};
      }
      std::vector<std::string> extensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
      std::vector<std::string> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
      return createConfiguration(
          applicationName,
          applicationVersionMajor,
          applicationVersionMinor,
          applicationVersionPatch,
          engineName,
          engineVersionMajor,
          engineVersionMinor,
          engineVersionPatch,
          validationLayersEnabled,
          validationLayers,
          extensions,
          deviceExtensions
      );
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Configuration Functions::createConfiguration(
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
  ) {
    try {
      Configuration configuration;
      configuration.applicationName = applicationName;
      configuration.applicationVersionMajor = applicationVersionMajor;
      configuration.applicationVersionMinor = applicationVersionMinor;
      configuration.applicationVersionPatch = applicationVersionPatch;
      configuration.engineName = engineName;
      configuration.engineVersionMajor = engineVersionMajor;
      configuration.engineVersionMinor = engineVersionMinor;
      configuration.engineVersionPatch = engineVersionPatch;
      configuration.validationLayersEnabled = validationLayersEnabled;
      configuration.validationLayers = StringVector(validationLayers);
      configuration.extensions = StringVector(extensions);
      configuration.deviceExtensions = StringVector(deviceExtensions);
      return configuration;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Logger Functions::createLogger() {
    try {
      return createLogger(std::cout);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Logger Functions::createLogger(std::ostream& stream) {
    try {
      std::function < void(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          std::string
      ) > function = [&stream](
          VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
          VkDebugUtilsMessageTypeFlagsEXT messageType,
          std::string message
      ) {
        //std::string function = std::string("(") + __FUNCTION__ + ")";

        std::string level;
        if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT == messageSeverity) {
          level = "[VERBOSE]";
        } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT == messageSeverity) {
          level = "[INFO]";
        } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT == messageSeverity) {
          level = "[WARNING]";
        } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT == messageSeverity) {
          level = "[ERROR]";
        } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT == messageSeverity) {
          level = "[MAX]";
        }

        std::string type;
        if (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT == messageType) {
          type = "(GENERAL)";
        } else if (VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT == messageType) {
          type = "(VALIDATION)";
        } else if (VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT == messageType) {
          type = "(PERFORMANCE)";
        } else if (VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT == messageType) {
          type = "(MAX)";
        }

        std::string line;

        //line += function;
        //line += " ";
        line += level;
        line += " ";
        line += type;
        line += " ";
        line += "validation layer:";
        line += " ";
        line += message;

        stream << line << std::endl;
      };

      return Logger(function);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Logger Functions::createLogger(
      const std::function<void(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          const std::string&
      )>& function
  ) {
    try {
      return Logger(function);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkInstance Functions::createInstance(Configuration& configuration, Logger& logger) {
    try {
      if (configuration.validationLayersEnabled && !checkValidationLayerSupport(configuration.validationLayers.values)) {
        throw std::runtime_error(CALL_INFO() + ": validation layers requested, but not available!");
      }

      VkApplicationInfo appInfo{};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = "Exqudens Application";
      appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.pEngineName = "Exqudens Engine";
      appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.apiVersion = VK_API_VERSION_1_0;

      VkInstanceCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      createInfo.pApplicationInfo = &appInfo;
      createInfo.enabledExtensionCount = static_cast<uint32_t>(configuration.extensions.pointers.size());
      createInfo.ppEnabledExtensionNames = configuration.extensions.pointers.data();

      // declared before if statement for to not be destroyed before call vkCreateInstance
      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
      if (configuration.validationLayersEnabled) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(configuration.validationLayers.pointers.size());
        createInfo.ppEnabledLayerNames = configuration.validationLayers.pointers.data();
        populateDebugMessengerCreateInfo(debugCreateInfo, logger);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
      } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
      }

      VkInstance instance;
      if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to create instance!");
      }

      return instance;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkDebugUtilsMessengerEXT Functions::createDebugUtilsMessenger(Logger& logger, VkInstance& instance) {
    try {
      VkDebugUtilsMessengerEXT debugUtilsMessenger;
      VkResult result;

      auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

      if (func != nullptr) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo, logger);
        result = func(instance, &createInfo, nullptr, &debugUtilsMessenger);
      } else {
        result = VK_ERROR_EXTENSION_NOT_PRESENT;
      }

      if (result != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to set up debug messenger!");
      }

      return debugUtilsMessenger;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  /*VkSurfaceKHR Functions::createSurface(GLFWwindow*& window, VkInstance& instance) {
    try {
      VkSurfaceKHR surface;
      if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to create window surface!");
      }
      return surface;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }*/

  VkPhysicalDevice Functions::createPhysicalDevice(VkInstance& instance, VkSurfaceKHR& surface, StringVector& deviceExtensions) {
    try {
      VkPhysicalDevice physicalDevice;
      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

      if (deviceCount == 0) {
        throw std::runtime_error(CALL_INFO() + ": failed to find GPUs with Vulkan support!");
      }

      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

      for (VkPhysicalDevice& object : devices) {
        if (isDeviceSuitable(object, surface, deviceExtensions)) {
          physicalDevice = object;
          break;
        }
      }

      if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error(CALL_INFO() + ": failed to find a suitable GPU!");
      }

      return physicalDevice;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Functions::setEnvironmentVariable(const std::string& name, const std::string& value) {
#ifdef _WIN32
    _putenv_s(name.c_str(), value.c_str());
#elif _WIN64
    _putenv_s(name.c_str(), value.c_str());
#endif
  }

  std::optional<std::string> Functions::getEnvironmentVariable(const std::string& name) {
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
  }

  bool Functions::checkValidationLayerSupport(const std::vector<std::string>& validationLayers) {
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

  void Functions::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& object, Logger& logger) {
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

  bool Functions::isDeviceSuitable(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, StringVector& deviceExtensions) {
    try {
      QueueFamilyIndices familyIndices = findQueueFamilies(physicalDevice, surface);

      bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice, deviceExtensions);

      bool swapChainAdequate = false;
      if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
      }

      VkPhysicalDeviceFeatures supportedFeatures;
      vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

      return familyIndices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  QueueFamilyIndices Functions::findQueueFamilies(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) {
    try {
      QueueFamilyIndices familyIndices;

      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

      int i = 0;
      for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          familyIndices.graphicsFamily = i;
        }

        if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
          familyIndices.transferFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

        if (presentSupport) {
          familyIndices.presentFamily = i;
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

  bool Functions::checkDeviceExtensionSupport(VkPhysicalDevice& physicalDevice, StringVector& deviceExtensions) {
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

  SwapChainSupportDetails Functions::querySwapChainSupport(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) {
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

  void Functions::destroyDebugUtilsMessenger(VkInstance& instance, VkDebugUtilsMessengerEXT& debugUtilsMessenger) {
    try {
      if (debugUtilsMessenger != nullptr) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        func(instance, debugUtilsMessenger, nullptr);
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Functions::destroyInstance(VkInstance& instance) {
    try {
      if (instance != nullptr) {
        vkDestroyInstance(instance, nullptr);
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

}
