#include "exqudens/vulkan/Functions.hpp"
#include "exqudens/vulkan/Macros.hpp"

#include <set>
#include <filesystem>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <cstdlib>

namespace exqudens::vulkan {

  Functions::Functions(
      std::function<VkSurfaceKHR(VkInstance&)> createSurfaceFunction
  ):
      createSurfaceFunction(std::move(createSurfaceFunction))
  {
  }

  Functions::Functions() = default;

  Functions::Functions(const Functions& object): Functions(
      object.createSurfaceFunction
  ) {
  }

  Functions::Functions(Functions&& object) noexcept: Functions() {
    swap(*this, object);
  }

  Functions& Functions::operator=(const Functions& object) {
    return *this = Functions(object);
  }

  Functions& Functions::operator=(Functions&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(Functions& first, Functions& second) {
    std::swap(first.createSurfaceFunction, second.createSurfaceFunction);
  }

  // create

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
      VkInstance instance = nullptr;

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

      if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS || instance == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to create instance!");
      }

      return instance;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkDebugUtilsMessengerEXT Functions::createDebugUtilsMessenger(VkInstance& instance, Logger& logger) {
    try {
      VkDebugUtilsMessengerEXT debugUtilsMessenger = nullptr;

      VkResult result;

      auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

      if (func != nullptr) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo, logger);
        result = func(instance, &createInfo, nullptr, &debugUtilsMessenger);
      } else {
        result = VK_ERROR_EXTENSION_NOT_PRESENT;
      }

      if (result != VK_SUCCESS || debugUtilsMessenger == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed create debug messenger!");
      }

      return debugUtilsMessenger;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkSurfaceKHR Functions::createSurface(VkInstance& instance) {
    try {
      VkSurfaceKHR surface = nullptr;

      try {
        surface = createSurfaceFunction(instance);
      } catch (const std::bad_function_call& e) {
        surface = nullptr;
      }

      if (surface == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed create surface!");
      }

      return surface;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkPhysicalDevice Functions::createPhysicalDevice(
      VkInstance& instance,
      Configuration& configuration,
      VkSurfaceKHR& surface
  ) {
    try {
      VkPhysicalDevice physicalDevice = nullptr;

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
        if (isDeviceSuitable(object, configuration, surface)) {
          physicalDevice = object;
          break;
        }
      }

      if (physicalDevice == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to find a suitable GPU!");
      }

      return physicalDevice;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkDevice Functions::createDevice(
      VkPhysicalDevice& physicalDevice,
      Configuration& configuration,
      VkSurfaceKHR& surface
  ) {
    try {
      VkDevice device = nullptr;

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

      std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

      std::set<uint32_t> uniqueQueueFamilies = familyIndices.uniqueQueueFamilies();
      float queuePriority = 1.0f;
      for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
      }

      VkPhysicalDeviceFeatures deviceFeatures{};
      deviceFeatures.samplerAnisotropy = VK_TRUE;

      VkDeviceCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

      createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
      createInfo.pQueueCreateInfos = queueCreateInfos.data();

      createInfo.pEnabledFeatures = &deviceFeatures;

      createInfo.enabledExtensionCount = static_cast<uint32_t>(configuration.deviceExtensions.pointers.size());
      createInfo.ppEnabledExtensionNames = configuration.deviceExtensions.pointers.data();

      if (configuration.validationLayersEnabled) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(configuration.validationLayers.pointers.size());
        createInfo.ppEnabledLayerNames = configuration.validationLayers.pointers.data();
      } else {
        createInfo.enabledLayerCount = 0;
      }

      if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS || device == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to create logical device!");
      }

      return device;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkQueue Functions::createQueue(
      QueueType type,
      VkPhysicalDevice& physicalDevice,
      Configuration& configuration,
      VkSurfaceKHR& surface,
      VkDevice& device,
      uint32_t queueIndex
  ) {
    try {
      VkQueue queue = nullptr;

      QueueFamilyIndices familyIndices = findQueueFamilies(
          physicalDevice,
          configuration.computeQueueFamilyRequired,
          configuration.transferQueueFamilyRequired,
          configuration.graphicsQueueFamilyRequired,
          surface
      );

      if (type == QueueType::COMPUTE) {
        if (!configuration.computeQueueFamilyRequired) {
          throw std::runtime_error(
              CALL_INFO()
              + ": failed to create queue, "
              + TO_STRING_SINGLE_QUOTES(QueueType::COMPUTE)
              + " queue not required in configuration!");
        }
        vkGetDeviceQueue(device, familyIndices.computeFamily.value(), queueIndex, &queue);
      } else if (type == QueueType::TRANSFER) {
        if (!configuration.transferQueueFamilyRequired) {
          throw std::runtime_error(
              CALL_INFO()
              + ": failed to create queue, "
              + TO_STRING_SINGLE_QUOTES(QueueType::TRANSFER)
              + " queue not required in configuration!");
        }
        vkGetDeviceQueue(device, familyIndices.transferFamily.value(), queueIndex, &queue);
      } else if (type == QueueType::GRAPHICS) {
        if (!configuration.graphicsQueueFamilyRequired) {
          throw std::runtime_error(
              CALL_INFO()
              + ": failed to create queue, "
              + TO_STRING_SINGLE_QUOTES(QueueType::GRAPHICS)
              + " queue not required in configuration!");
        }
        vkGetDeviceQueue(device, familyIndices.graphicsFamily.value(), queueIndex, &queue);
      } else if (type == QueueType::PRESENT) {
        if (!configuration.presentQueueFamilyRequired || surface == nullptr) {
          throw std::runtime_error(
              CALL_INFO()
              + ": failed to create queue, "
              + TO_STRING_SINGLE_QUOTES(QueueType::PRESENT)
              + " queue not required in configuration!");
        }
        vkGetDeviceQueue(device, familyIndices.presentFamily.value(), queueIndex, &queue);
      }

      if (queue == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to create queue!");
      }

      return queue;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  // utility

  void Functions::setEnvironmentVariable(const std::string& name, const std::string& value) {
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

  std::optional<std::string> Functions::getEnvironmentVariable(const std::string& name) {
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

  bool Functions::isDeviceSuitable(
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

  QueueFamilyIndices Functions::findQueueFamilies(
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

  // destroy

  void Functions::destroyDevice(VkDevice& device) {
    try {
      if (device != nullptr) {
        vkDestroyDevice(device, nullptr);
        device = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Functions::destroyPhysicalDevice(VkPhysicalDevice& physicalDevice) {
    try {
      physicalDevice = nullptr;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Functions::destroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT& debugUtilsMessenger, VkInstance& instance) {
    try {
      if (debugUtilsMessenger != nullptr) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        func(instance, debugUtilsMessenger, nullptr);
        debugUtilsMessenger = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Functions::destroyInstance(VkInstance& instance) {
    try {
      if (instance != nullptr) {
        vkDestroyInstance(instance, nullptr);
        instance = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Functions::~Functions() = default;

}
