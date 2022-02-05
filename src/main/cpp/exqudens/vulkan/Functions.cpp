#include "exqudens/vulkan/Functions.hpp"
#include "exqudens/vulkan/Macros.hpp"

#include <filesystem>
#include <stdexcept>
#include <iostream>

namespace exqudens::vulkan {

  Configuration Functions::createConfiguration() {
    try {
      bool validationLayersEnabled = true;
      std::vector<std::string> validationLayers = {"VK_LAYER_KHRONOS_validation"};
      std::vector<std::string> extensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
      std::vector<std::string> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
      return createConfiguration(
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
      std::vector<std::string> validationLayers;
      if (validationLayersEnabled) {
        validationLayers = {"VK_LAYER_KHRONOS_validation"};
      }
      std::vector<std::string> extensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
      std::vector<std::string> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
      return createConfiguration(
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
      const bool& validationLayersEnabled,
      const std::vector<std::string>& validationLayers,
      const std::vector<std::string>& extensions,
      const std::vector<std::string>& deviceExtensions
  ) {
    try {
      Configuration configuration;
      configuration.validationLayersEnabled = validationLayersEnabled;
      configuration.validationLayers = StringVector(validationLayers);
      configuration.extensions = StringVector(extensions);
      configuration.deviceExtensions = StringVector(deviceExtensions);
      return configuration;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  std::map<std::string, std::string> Functions::createEnvironmentVariables(const std::string& executableDirPath) {
    std::map<std::string, std::string> environmentVariables;
    environmentVariables["VK_LAYER_PATH"] = std::filesystem::path(executableDirPath).make_preferred().string();
    return environmentVariables;
  }

  Logger Functions::createLogger() {
    return createLogger(std::cout);
  }

  Logger Functions::createLogger(std::ostream& stream) {
    std::function<void(
        VkDebugUtilsMessageSeverityFlagBitsEXT,
        VkDebugUtilsMessageTypeFlagsEXT,
        std::string
    )> function = [&stream](
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
  }

  Logger Functions::createLogger(
      const std::function<void(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          std::string
      )>& logFunction
  ) {
    std::function<void(
        VkDebugUtilsMessageSeverityFlagBitsEXT,
        VkDebugUtilsMessageTypeFlagsEXT,
        std::string
    )> function = logFunction;
    return Logger(function);
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

  bool Functions::checkValidationLayerSupport(const std::vector<std::string>& validationLayers) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const std::string& layerName : validationLayers) {
      bool layerFound = false;

      for (const auto& layerProperties : availableLayers) {
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
  }

  void Functions::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& object, Logger& logger) {
    object = {};
    object.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    object.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    object.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    object.pfnUserCallback = &Logger::call;
    object.pUserData = &logger;
  }

}
