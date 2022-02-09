#include "exqudens/vulkan/function/CreateFunctions.hpp"
#include "exqudens/vulkan/Macros.hpp"

#include <filesystem>
#include <stdexcept>
#include <utility>
#include <iostream>

namespace exqudens::vulkan {

  CreateFunctions::CreateFunctions(
      UtilFunctions* utilFunctions
  ):
      utilFunctions(utilFunctions)
  {
  }

  CreateFunctions::CreateFunctions() = default;

  std::map<std::string, std::string> CreateFunctions::createEnvironmentVariables(const std::string& executableDirPath) {
    try {
      std::map<std::string, std::string> environmentVariables;
      environmentVariables["VK_LAYER_PATH"] = std::filesystem::path(executableDirPath).make_preferred().string();
      return environmentVariables;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Configuration CreateFunctions::createConfiguration() {
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
      StringVector validationLayers = StringVector({"VK_LAYER_KHRONOS_validation"});
      StringVector extensions = StringVector({VK_EXT_DEBUG_UTILS_EXTENSION_NAME});
      StringVector deviceExtensions = StringVector({VK_KHR_SWAPCHAIN_EXTENSION_NAME});
      bool computeQueueFamilyRequired = true;
      bool transferQueueFamilyRequired = true;
      bool graphicsQueueFamilyRequired = true;
      bool presentQueueFamilyRequired = true;
      bool anisotropyRequired = true;
      return {
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
          deviceExtensions,
          computeQueueFamilyRequired,
          transferQueueFamilyRequired,
          graphicsQueueFamilyRequired,
          presentQueueFamilyRequired,
          anisotropyRequired
      };
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Logger CreateFunctions::createLogger() {
    try {
      return createLogger(std::cout);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Logger CreateFunctions::createLogger(std::ostream& stream) {
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

  Logger CreateFunctions::createLogger(
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

  VkInstance CreateFunctions::createInstance(Configuration& configuration, Logger& logger) {
    try {
      VkInstance instance = nullptr;

      if (utilFunctions == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": " + TO_STRING_SINGLE_QUOTES(utilFunctions) + " is null!");
      }

      if (configuration.validationLayersEnabled) {
        if (!utilFunctions->checkValidationLayerSupport(configuration.validationLayers.values)) {
          throw std::runtime_error(CALL_INFO() + ": validation layers requested, but not available!");
        }
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
        utilFunctions->populateDebugMessengerCreateInfo(debugCreateInfo, logger);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
      } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
      }

      if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to create instance!");
      }

      return instance;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkDebugUtilsMessengerEXT CreateFunctions::createDebugUtilsMessenger(VkInstance& instance, Logger& logger) {
    try {
      VkDebugUtilsMessengerEXT debugUtilsMessenger = nullptr;

      if (utilFunctions == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": 'utilFunctions' is null!");
      }

      VkResult result;

      auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

      if (func != nullptr) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        utilFunctions->populateDebugMessengerCreateInfo(createInfo, logger);
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

  VkPhysicalDevice CreateFunctions::createPhysicalDevice(VkInstance& instance, Configuration& configuration) {
    try {
      VkSurfaceKHR surface = nullptr;
      return createPhysicalDevice(
          instance,
          configuration,
          surface
      );
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkPhysicalDevice CreateFunctions::createPhysicalDevice(
      VkInstance& instance,
      Configuration& configuration,
      VkSurfaceKHR& surface
  ) {
    try {
      VkPhysicalDevice physicalDevice = nullptr;

      if (utilFunctions == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": " + TO_STRING_SINGLE_QUOTES(utilFunctions) + " is null!");
      }

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

      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

      if (deviceCount == 0) {
        throw std::runtime_error(CALL_INFO() + ": failed to find GPUs with Vulkan support!");
      }

      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

      for (VkPhysicalDevice& object : devices) {
        if (
            utilFunctions->isDeviceSuitable(
                object,
                configuration,
                surface
            )
        ) {
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

  CreateFunctions::~CreateFunctions() = default;

}
