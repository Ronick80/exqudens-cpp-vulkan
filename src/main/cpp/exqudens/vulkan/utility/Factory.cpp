#include "exqudens/vulkan/utility/Factory.hpp"
#include "exqudens/vulkan/utility/Macros.hpp"

#include <array>
#include <set>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <cstdlib>

namespace exqudens::vulkan {

  Factory::Factory(
      std::function<VkSurfaceKHR(VkInstance&)> createSurfaceFunction
  ):
      createSurfaceFunction(std::move(createSurfaceFunction))
  {
  }

  Factory::Factory() = default;

  Factory::Factory(const Factory& object): Factory(
      object.createSurfaceFunction
  ) {
  }

  Factory::Factory(Factory&& object) noexcept: Factory() {
    swap(*this, object);
  }

  Factory& Factory::operator=(const Factory& object) {
    return *this = Factory(object);
  }

  Factory& Factory::operator=(Factory&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(Factory& first, Factory& second) {
    std::swap(first.createSurfaceFunction, second.createSurfaceFunction);
  }

  // utility

  void Factory::setEnvironmentVariable(const std::string& name, const std::string& value) {
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

  std::optional<std::string> Factory::getEnvironmentVariable(const std::string& name) {
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

  std::vector<char> Factory::readFile(const std::string& path) {
    try {
      std::ifstream file(path, std::ios::ate | std::ios::binary);

      if (!file.is_open()) {
        throw std::runtime_error(CALL_INFO() + ": failed to open file: '" + path + "'!");
      }

      std::streamsize fileSize = file.tellg();
      std::vector<char> buffer(fileSize);

      file.seekg(0);
      file.read(buffer.data(), fileSize);

      file.close();

      return buffer;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  bool Factory::checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {
    try {
      uint32_t layerCount;
      vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

      std::vector<VkLayerProperties> availableLayers(layerCount);
      vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

      for (const std::string& layerName: validationLayers) {
        bool layerFound = false;

        for (const VkLayerProperties& layerProperties : availableLayers) {
          if (std::string(layerName) == std::string(layerProperties.layerName)) {
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

  void Factory::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& object, Logger& logger) {
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

  QueueFamilyIndexInfo Factory::findQueueFamilies(
      VkPhysicalDevice& physicalDevice,
      bool computeFamilyRequired,
      bool transferFamilyRequired,
      bool graphicsFamilyRequired,
      VkSurfaceKHR& surface
  ) {
    try {
      QueueFamilyIndexInfo result;
      result.computeFamilyRequired = computeFamilyRequired;
      result.transferFamilyRequired = transferFamilyRequired;
      result.graphicsFamilyRequired = graphicsFamilyRequired;
      result.presentFamilyRequired = surface != nullptr;

      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

      int i = 0;
      for (const auto& queueFamily : queueFamilies) {
        if (result.computeFamilyRequired) {
          if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            result.computeFamily = i;
          }
        }

        if (result.transferFamilyRequired) {
          if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
            result.transferFamily = i;
          }
        }

        if (result.graphicsFamilyRequired) {
          if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            result.graphicsFamily = i;
          }
        }

        if (result.presentFamilyRequired) {
          VkBool32 presentSupport = false;
          vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
          if (presentSupport) {
            result.presentFamily = i;
          }
        }

        bool computeFamilyComplete = !result.computeFamilyRequired || result.computeFamily.has_value();
        bool transferFamilyComplete = !result.transferFamilyRequired || result.transferFamily.has_value();
        bool graphicsFamilyComplete = !result.graphicsFamilyRequired || result.graphicsFamily.has_value();
        bool presentFamilyComplete = !result.presentFamilyRequired || result.presentFamily.has_value();
        bool complete = computeFamilyComplete && transferFamilyComplete && graphicsFamilyComplete && presentFamilyComplete;

        if (complete) {
          result.complete = complete;
          break;
        }

        i++;
      }

      if (result.complete) {
        if (result.computeFamilyRequired) {
          result.uniqueQueueFamilyIndices.insert(result.computeFamily.value());
        }
        if (result.transferFamilyRequired) {
          result.uniqueQueueFamilyIndices.insert(result.transferFamily.value());
        }
        if (result.graphicsFamilyRequired) {
          result.uniqueQueueFamilyIndices.insert(result.graphicsFamily.value());
        }
        if (result.presentFamilyRequired) {
          result.uniqueQueueFamilyIndices.insert(result.presentFamily.value());
        }
      }

      return result;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  bool Factory::checkDeviceExtensionSupport(VkPhysicalDevice& physicalDevice, const std::vector<const char*>& deviceExtensions) {
    try {
      uint32_t extensionCount;
      vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

      std::vector<VkExtensionProperties> availableExtensions(extensionCount);
      vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

      std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

      for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
      }

      return requiredExtensions.empty();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  SwapChainSupportDetails Factory::querySwapChainSupport(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) {
    try {
      if (surface == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to get swap chain support details!");
      }

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

  VkSurfaceFormatKHR Factory::chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats) {
    try {
      for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
          return availableFormat;
        }
      }
      return availableFormats[0];
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkPresentModeKHR Factory::chooseSwapPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes) {
    try {
      for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
          return availablePresentMode;
        }
      }
      return VK_PRESENT_MODE_FIFO_KHR;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkExtent2D Factory::chooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities, const int& width, const int& height) {
    try {
      if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
      } else {
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  uint32_t Factory::findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    try {
      VkPhysicalDeviceMemoryProperties memProperties;
      vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

      for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
          return i;
        }
      }

      throw std::runtime_error(CALL_INFO() + ": failed to find suitable memory type!");
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  // create

  std::map<std::string, std::string> Factory::createEnvironmentVariables(const std::string& executableDirPath) {
    try {
      std::map<std::string, std::string> environmentVariables;
      environmentVariables["VK_LAYER_PATH"] = std::filesystem::path(executableDirPath).make_preferred().string();
      return environmentVariables;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Configuration Factory::createConfiguration() {
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
      std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
      std::vector<const char*> extensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
      std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
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

  Logger Factory::createLogger() {
    try {
      return createLogger(std::cout);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Logger Factory::createLogger(std::ostream& stream) {
    try {
      std::function < void(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          std::string
      ) > function = [&stream](
          VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
          VkDebugUtilsMessageTypeFlagsEXT messageType,
          const std::string& message
      ) {
        //std::string function = std::string("(") + __FUNCTION__ + ")";

        std::string level;
        if (VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT == messageSeverity) {
          level = "[VERBOSE]";
        } else if (VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT == messageSeverity) {
          level = "[INFO]";
        } else if (VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT == messageSeverity) {
          level = "[WARNING]";
        } else if (VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT == messageSeverity) {
          level = "[ERROR]";
        } else if (VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT == messageSeverity) {
          level = "[MAX]";
        }

        std::string type;
        if (VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT == messageType) {
          type = "(GENERAL)";
        } else if (VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT == messageType) {
          type = "(VALIDATION)";
        } else if (VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT == messageType) {
          type = "(PERFORMANCE)";
        } else if (VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT == messageType) {
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

  Logger Factory::createLogger(
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

  VkInstance Factory::createInstance(Configuration& configuration, Logger& logger) {
    try {
      VkInstance instance = nullptr;

      if (configuration.validationLayersEnabled && !checkValidationLayerSupport(configuration.validationLayers)) {
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
      createInfo.enabledExtensionCount = static_cast<uint32_t>(configuration.extensions.size());
      createInfo.ppEnabledExtensionNames = configuration.extensions.data();

      // declared before if statement for to not be destroyed before call vkCreateInstance
      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
      if (configuration.validationLayersEnabled) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(configuration.validationLayers.size());
        createInfo.ppEnabledLayerNames = configuration.validationLayers.data();
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

  VkDebugUtilsMessengerEXT Factory::createDebugUtilsMessenger(VkInstance& instance, Logger& logger) {
    try {
      VkDebugUtilsMessengerEXT debugUtilsMessenger = nullptr;

      VkResult result;

      auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

      if (func != nullptr) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
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

  VkSurfaceKHR Factory::createSurface(VkInstance& instance) {
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

  PhysicalDevice Factory::createPhysicalDevice(VkInstance& instance, Configuration& configuration) {
    try {
      VkSurfaceKHR surface = nullptr;
      return createPhysicalDevice(instance, configuration, surface);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  PhysicalDevice Factory::createPhysicalDevice(
      VkInstance& instance,
      Configuration& configuration,
      VkSurfaceKHR& surface
  ) {
    try {
      VkPhysicalDevice physicalDevice = nullptr;

      if (configuration.presentQueueFamilyRequired && surface == nullptr) {
        throw std::runtime_error(
            CALL_INFO()
            + ": '"
            + TO_STRING(configuration.presentQueueFamilyRequired)
            + "': true but '"
            + TO_STRING(surface)
            + "' is null!"
        );
      }

      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

      if (deviceCount == 0) {
        throw std::runtime_error(CALL_INFO() + ": failed to find GPUs with Vulkan support!");
      }

      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

      QueueFamilyIndexInfo queueFamilyIndexInfo = {};
      std::optional<SwapChainSupportDetails> swapChainSupportDetails = {};

      for (VkPhysicalDevice& object : devices) {

        QueueFamilyIndexInfo familyIndices = findQueueFamilies(
            object,
            configuration.computeQueueFamilyRequired,
            configuration.transferQueueFamilyRequired,
            configuration.graphicsQueueFamilyRequired,
            surface
        );

        bool extensionsSupported = checkDeviceExtensionSupport(object, configuration.deviceExtensions);

        bool swapChainAdequate = surface == nullptr;
        std::optional<SwapChainSupportDetails> optionalSwapChainSupport = {};
        if (extensionsSupported && surface != nullptr) {
          SwapChainSupportDetails swapChainSupport = querySwapChainSupport(object, surface);
          swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
          optionalSwapChainSupport = swapChainSupport;
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(object, &supportedFeatures);

        bool anisotropyAdequate = true;
        if (configuration.anisotropyRequired) {
          anisotropyAdequate = supportedFeatures.samplerAnisotropy;
        }

        bool isDeviceSuitable = familyIndices.complete &&
               extensionsSupported &&
               swapChainAdequate &&
               anisotropyAdequate;

        if (isDeviceSuitable) {
          queueFamilyIndexInfo = familyIndices;
          swapChainSupportDetails = optionalSwapChainSupport;
          physicalDevice = object;
          break;
        }
      }

      if (physicalDevice == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to find a suitable GPU!");
      }

      PhysicalDevice result;
      result.queueFamilyIndexInfo = queueFamilyIndexInfo;
      result.swapChainSupportDetails = swapChainSupportDetails;
      result.value = physicalDevice;
      return result;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkDevice Factory::createDevice(
      VkPhysicalDevice& physicalDevice,
      Configuration& configuration,
      QueueFamilyIndexInfo& queueFamilyIndexInfo
  ) {
    try {
      VkDevice device = nullptr;

      std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

      float queuePriority = 1.0f;
      for (uint32_t queueFamily : queueFamilyIndexInfo.uniqueQueueFamilyIndices) {
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

      createInfo.enabledExtensionCount = static_cast<uint32_t>(configuration.deviceExtensions.size());
      createInfo.ppEnabledExtensionNames = configuration.deviceExtensions.data();

      if (configuration.validationLayersEnabled) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(configuration.validationLayers.size());
        createInfo.ppEnabledLayerNames = configuration.validationLayers.data();
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

  VkQueue Factory::createQueue(
      VkDevice& device,
      uint32_t queueFamilyIndex,
      uint32_t queueIndex
  ) {
    try {
      VkQueue queue = nullptr;

      vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, &queue);

      if (queue == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to create compute queue!");
      }

      return queue;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  SwapChain Factory::createSwapChain(
      SwapChainSupportDetails& swapChainSupport,
      QueueFamilyIndexInfo& queueFamilyIndexInfo,
      VkSurfaceKHR& surface,
      VkDevice& device,
      const int& width,
      const int& height
  ) {
    try {
      if (surface == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": surface is null failed to create swap chain!");
      }

      VkSwapchainKHR swapChain = nullptr;

      VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
      VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
      VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

      uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

      if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
      }

      VkSwapchainCreateInfoKHR createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      createInfo.surface = surface;

      createInfo.minImageCount = imageCount;
      createInfo.imageFormat = surfaceFormat.format;
      createInfo.imageColorSpace = surfaceFormat.colorSpace;
      createInfo.imageExtent = extent;
      createInfo.imageArrayLayers = 1;
      createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

      std::vector<uint32_t> queueFamilyIndices = {
          queueFamilyIndexInfo.graphicsFamily.value(),
          queueFamilyIndexInfo.presentFamily.value()
      };

      if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());;
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
      } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      }

      createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
      createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
      createInfo.presentMode = presentMode;
      createInfo.clipped = VK_TRUE;

      createInfo.oldSwapchain = nullptr;

      if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS || swapChain == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to create swap chain!");
      }

      SwapChain result;
      result.format = surfaceFormat.format;
      result.extent = extent;
      result.width = width;
      result.height = height;
      result.value = swapChain;
      return result;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Buffer Factory::createBuffer(
      VkPhysicalDevice& physicalDevice,
      VkDevice& device,
      VkDeviceSize size,
      VkBufferUsageFlags usage,
      VkMemoryPropertyFlags properties
  ) {
    try {
      VkBuffer buffer = nullptr;

      VkBufferCreateInfo bufferInfo{};
      bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufferInfo.size = size;
      bufferInfo.usage = usage;
      bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      if (
          vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS
          || buffer == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create buffer!");
      }

      VkDeviceMemory bufferMemory = nullptr;

      VkMemoryRequirements memRequirements;
      vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

      if (
          vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS
          || bufferMemory == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to allocate buffer memory!");
      }

      vkBindBufferMemory(device, buffer, bufferMemory, 0);

      Buffer result;
      result.value = buffer;
      result.memory = bufferMemory;
      return result;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Image Factory::createImage(
      VkPhysicalDevice& physicalDevice,
      VkDevice& device,
      uint32_t width,
      uint32_t height,
      VkFormat format,
      VkImageTiling tiling,
      VkImageUsageFlags usage,
      VkMemoryPropertyFlags properties
  ) {
    try {
      VkImageCreateInfo imageInfo = {};
      imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
      imageInfo.imageType = VK_IMAGE_TYPE_2D;
      imageInfo.extent.width = width;
      imageInfo.extent.height = height;
      imageInfo.extent.depth = 1;
      imageInfo.mipLevels = 1;
      imageInfo.arrayLayers = 1;
      imageInfo.format = format;
      imageInfo.tiling = tiling;
      imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imageInfo.usage = usage;
      imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      VkImage image = nullptr;

      if (
          vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS
          || image == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create image!");
      }

      VkDeviceMemory imageMemory = nullptr;

      VkMemoryRequirements memRequirements;
      vkGetImageMemoryRequirements(device, image, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
      }

      vkBindImageMemory(device, image, imageMemory, 0);

      Image result;
      result.value = image;
      result.memory = imageMemory;
      return result;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  std::vector<Image> Factory::createImages(
      VkPhysicalDevice& physicalDevice,
      VkDevice& device,
      uint32_t width,
      uint32_t height,
      VkFormat format,
      VkImageTiling tiling,
      VkImageUsageFlags usage,
      VkMemoryPropertyFlags properties,
      std::size_t size
  ) {
    try {
      std::vector<Image> images;
      images.resize(size);
      for (std::size_t i = 0; i < size; i++) {
        images[i] = createImage(physicalDevice, device, width, height, format, tiling, usage, properties);
      }
      return images;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  std::vector<VkImage> Factory::createSwapChainImages(
      VkDevice& device,
      VkSwapchainKHR& swapChain
  ) {
    try {
      uint32_t imageCount = 0;
      std::vector<VkImage> swapChainImages;
      vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
      swapChainImages.resize(imageCount);
      vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
      return swapChainImages;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkImageView Factory::createImageView(VkDevice& device, VkImage& image, VkFormat& format) {
    try {
      VkImageViewCreateInfo viewInfo = {};
      viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      viewInfo.image = image;
      viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      viewInfo.format = format;
      //viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      //viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      //viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      //viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
      viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      viewInfo.subresourceRange.baseMipLevel = 0;
      viewInfo.subresourceRange.levelCount = 1;
      viewInfo.subresourceRange.baseArrayLayer = 0;
      viewInfo.subresourceRange.layerCount = 1;

      VkImageView imageView = nullptr;

      if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS || imageView == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to create texture image view!");
      }

      return imageView;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  std::vector<VkImageView> Factory::createImageViews(
      VkDevice& device,
      std::vector<VkImage>& images,
      VkFormat& format
  ) {
    try {
      std::vector<VkImageView> imageViews;

      imageViews.resize(images.size());
      for (uint32_t i = 0; i < images.size(); i++) {
        imageViews[i] = createImageView(device, images[i], format);
      }

      return imageViews;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkRenderPass Factory::createRenderPass(VkDevice& device, VkFormat& format) {
    try {
      VkRenderPass renderPass = nullptr;

      VkAttachmentDescription colorAttachment = {};
      colorAttachment.format = format;
      colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
      colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

      VkAttachmentReference colorAttachmentRef = {};
      colorAttachmentRef.attachment = 0;
      colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

      VkSubpassDescription subPass = {};
      subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      subPass.colorAttachmentCount = 1;
      subPass.pColorAttachments = &colorAttachmentRef;

      VkSubpassDependency dependency = {};
      dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
      dependency.dstSubpass = 0;
      dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      dependency.srcAccessMask = 0;
      dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

      VkRenderPassCreateInfo renderPassInfo = {};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      renderPassInfo.attachmentCount = 1;
      renderPassInfo.pAttachments = &colorAttachment;
      renderPassInfo.subpassCount = 1;
      renderPassInfo.pSubpasses = &subPass;
      renderPassInfo.dependencyCount = 1;
      renderPassInfo.pDependencies = &dependency;

      if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS || renderPass == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to create render pass!");
      }

      return renderPass;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkDescriptorSetLayout Factory::createDescriptorSetLayout(VkDevice& device) {
    try {
      VkDescriptorSetLayout descriptorSetLayout = nullptr;

      VkDescriptorSetLayoutBinding uboLayoutBinding{};
      uboLayoutBinding.binding = 0;
      uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      uboLayoutBinding.descriptorCount = 1;
      uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
      uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

      VkDescriptorSetLayoutBinding samplerLayoutBinding{};
      samplerLayoutBinding.binding = 1;
      samplerLayoutBinding.descriptorCount = 1;
      samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      samplerLayoutBinding.pImmutableSamplers = nullptr;
      samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

      std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};

      VkDescriptorSetLayoutCreateInfo layoutInfo{};
      layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
      layoutInfo.pBindings = bindings.data();

      if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to create descriptor set layout!");
      }

      return descriptorSetLayout;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Shader Factory::createShader(
      VkDevice& device,
      const std::string& path
  ) {
    try {
      VkShaderModule shaderModule = nullptr;

      const std::vector<char>& code = readFile(path);

      if (code.empty()) {
        throw std::runtime_error(CALL_INFO() + ": failed to create shader module code is empty!");
      }

      VkShaderModuleCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
      createInfo.codeSize = code.size();
      createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

      if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS || shaderModule == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to create shader module!");
      }

      VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
      pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

      if (path.ends_with(".vert.spv")) {
        pipelineShaderStageCreateInfo.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
      } else if (path.ends_with(".frag.spv")) {
        pipelineShaderStageCreateInfo.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
      } else {
        throw std::invalid_argument(CALL_INFO() + ": '" + path + "' failed to create shader!");
      }

      pipelineShaderStageCreateInfo.module = shaderModule;
      pipelineShaderStageCreateInfo.pName = "main";

      Shader shader;

      shader.shaderModule = shaderModule;
      shader.pipelineShaderStageCreateInfo = pipelineShaderStageCreateInfo;

      return shader;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Pipeline Factory::createGraphicsPipeline(
      VkDevice& device,
      VkExtent2D& extent,
      const std::vector<std::string>& shaderPaths,
      VkRenderPass& renderPass
  ) {
    try {
      VkFrontFace frontFace = VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
      VkDescriptorSetLayout descriptorSetLayout = nullptr;
      std::optional<VkVertexInputBindingDescription> bindingDescription = {}; //Vertex::getBindingDescription();
      std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {}; //Vertex::getAttributeDescriptions();
      return createGraphicsPipeline(
          device,
          extent,
          shaderPaths,
          renderPass,
          frontFace,
          descriptorSetLayout,
          bindingDescription,
          attributeDescriptions
      );
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Pipeline Factory::createGraphicsPipeline(
      VkDevice& device,
      VkExtent2D& extent,
      const std::vector<std::string>& shaderPaths,
      VkRenderPass& renderPass,
      VkFrontFace frontFace,
      VkDescriptorSetLayout& descriptorSetLayout,
      std::optional<VkVertexInputBindingDescription> bindingDescription,
      std::vector<VkVertexInputAttributeDescription> attributeDescriptions
  ) {
    try {
      VkPipelineLayout pipelineLayout = nullptr;

      VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
      vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

      vertexInputInfo.vertexBindingDescriptionCount = bindingDescription.has_value() ? 1 : 0;
      vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
      vertexInputInfo.pVertexBindingDescriptions = bindingDescription.has_value() ? &bindingDescription.value() : nullptr;
      vertexInputInfo.pVertexAttributeDescriptions = !attributeDescriptions.empty() ? attributeDescriptions.data() : nullptr;

      VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
      inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
      inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
      inputAssembly.primitiveRestartEnable = VK_FALSE;

      VkViewport viewport = {};
      viewport.x = 0.0f;
      viewport.y = 0.0f;
      viewport.width = (float) extent.width;
      viewport.height = (float) extent.height;
      viewport.minDepth = 0.0f;
      viewport.maxDepth = 1.0f;

      VkRect2D scissor = {};
      scissor.offset = {0, 0};
      scissor.extent = extent;

      VkPipelineViewportStateCreateInfo viewportState = {};
      viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
      viewportState.viewportCount = 1;
      viewportState.pViewports = &viewport;
      viewportState.scissorCount = 1;
      viewportState.pScissors = &scissor;

      VkPipelineRasterizationStateCreateInfo rasterizer = {};
      rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
      rasterizer.depthClampEnable = VK_FALSE;
      rasterizer.rasterizerDiscardEnable = VK_FALSE;
      rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
      rasterizer.lineWidth = 1.0f;
      rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
      rasterizer.frontFace = frontFace; // black screen WARNING!!!
      rasterizer.depthBiasEnable = VK_FALSE;
      rasterizer.depthBiasConstantFactor = 0.0f; // Optional
      rasterizer.depthBiasClamp = 0.0f; // Optional
      rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

      VkPipelineMultisampleStateCreateInfo multisampling = {};
      multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
      multisampling.sampleShadingEnable = VK_FALSE;
      multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
      multisampling.minSampleShading = 1.0f; // Optional
      multisampling.pSampleMask = nullptr; // Optional
      multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
      multisampling.alphaToOneEnable = VK_FALSE; // Optional

      VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
      colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
      colorBlendAttachment.blendEnable = VK_FALSE;
      colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
      colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
      colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
      colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
      colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
      colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

      VkPipelineColorBlendStateCreateInfo colorBlending = {};
      colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      colorBlending.logicOpEnable = VK_FALSE;
      colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
      colorBlending.attachmentCount = 1;
      colorBlending.pAttachments = &colorBlendAttachment;
      colorBlending.blendConstants[0] = 0.0f; // Optional
      colorBlending.blendConstants[1] = 0.0f; // Optional
      colorBlending.blendConstants[2] = 0.0f; // Optional
      colorBlending.blendConstants[3] = 0.0f; // Optional

      VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount = descriptorSetLayout != nullptr ? 1 : 0;
      pipelineLayoutInfo.pSetLayouts = descriptorSetLayout != nullptr ? &descriptorSetLayout : nullptr;
      pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
      pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

      if (
          vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS
          || pipelineLayout == nullptr
          ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create pipeline layout!");
      }

      VkPipeline pipeline = nullptr;

      std::vector<Shader> shaders;
      shaders.resize(shaderPaths.size());
      std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
      shaderStages.resize(shaders.size());
      for (std::size_t i = 0; i < shaderPaths.size(); i++) {
        shaders[i] = createShader(device, shaderPaths[i]);
        shaderStages[i] = shaders[i].pipelineShaderStageCreateInfo;
      }

      VkGraphicsPipelineCreateInfo pipelineInfo{};
      pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
      pipelineInfo.pStages = shaderStages.data();
      pipelineInfo.pVertexInputState = &vertexInputInfo;
      pipelineInfo.pInputAssemblyState = &inputAssembly;
      pipelineInfo.pViewportState = &viewportState;
      pipelineInfo.pRasterizationState = &rasterizer;
      pipelineInfo.pMultisampleState = &multisampling;
      pipelineInfo.pDepthStencilState = nullptr; // Optional
      pipelineInfo.pColorBlendState = &colorBlending;
      pipelineInfo.pDynamicState = nullptr; // Optional
      pipelineInfo.layout = pipelineLayout;
      pipelineInfo.renderPass = renderPass;
      pipelineInfo.subpass = 0;
      pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
      pipelineInfo.basePipelineIndex = -1; // Optional

      if (
          vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS
          || pipeline == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create graphics pipeline!");
      }

      for (Shader& shader : shaders) {
        destroyShader(shader, device);
      }

      Pipeline result;
      result.layout = pipelineLayout;
      result.value = pipeline;
      return result;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkFramebuffer Factory::createFrameBuffer(
      VkDevice& device,
      VkImageView& imageView,
      VkRenderPass& renderPass,
      const int& width,
      const int& height
  ) {
    try {
      VkFramebuffer frameBuffer = nullptr;

      std::vector<VkImageView> attachments = {
          imageView
      };

      VkFramebufferCreateInfo frameBufferInfo{};
      frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      frameBufferInfo.renderPass = renderPass;
      frameBufferInfo.attachmentCount = 1;
      frameBufferInfo.pAttachments = attachments.data();
      frameBufferInfo.width = width;
      frameBufferInfo.height = height;
      frameBufferInfo.layers = 1;

      if (
          vkCreateFramebuffer(device, &frameBufferInfo, nullptr, &frameBuffer) != VK_SUCCESS
          || frameBuffer == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create frame buffer!");
      }

      return frameBuffer;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  std::vector<VkFramebuffer> Factory::createFrameBuffers(
      VkDevice& device,
      std::vector<VkImageView>& imageViews,
      VkRenderPass& renderPass,
      const int& width,
      const int& height
  ) {
    try {
      std::vector<VkFramebuffer> frameBuffers;
      frameBuffers.resize(imageViews.size());
      for (std::size_t i = 0; i < imageViews.size(); i++) {
        frameBuffers[i] = createFrameBuffer(device, imageViews[i], renderPass, width, height);
      }
      return frameBuffers;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkDescriptorPool Factory::createDescriptorPool(VkDevice& device, std::size_t size) {
    try {
      VkDescriptorPool descriptorPool = nullptr;

      std::array<VkDescriptorPoolSize, 2> poolSizes{};

      poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      poolSizes[0].descriptorCount = static_cast<uint32_t>(size);

      poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      poolSizes[1].descriptorCount = static_cast<uint32_t>(size);

      VkDescriptorPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
      poolInfo.pPoolSizes = poolSizes.data();
      poolInfo.maxSets = static_cast<uint32_t>(size);
      poolInfo.flags = 0; // Optional

      if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to create descriptor pool!");
      }

      return descriptorPool;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkCommandPool Factory::createComputeCommandPool(
      VkPhysicalDevice& physicalDevice,
      Configuration& configuration,
      VkSurfaceKHR& surface,
      VkDevice& device
  ) {
    try {
      VkCommandPool commandPool = nullptr;

      QueueFamilyIndexInfo familyIndices = findQueueFamilies(
          physicalDevice,
          configuration.computeQueueFamilyRequired,
          configuration.transferQueueFamilyRequired,
          configuration.graphicsQueueFamilyRequired,
          surface
      );

      VkCommandPoolCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      createInfo.queueFamilyIndex = familyIndices.computeFamily.value();
      createInfo.flags = 0; // Optional

      if (
          vkCreateCommandPool(device, &createInfo, nullptr, &commandPool) != VK_SUCCESS
          || commandPool == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create compute command pool!");
      }

      return commandPool;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkCommandPool Factory::createTransferCommandPool(
      VkPhysicalDevice& physicalDevice,
      Configuration& configuration,
      VkSurfaceKHR& surface,
      VkDevice& device
  ) {
    try {
      VkCommandPool commandPool = nullptr;

      QueueFamilyIndexInfo familyIndices = findQueueFamilies(
          physicalDevice,
          configuration.computeQueueFamilyRequired,
          configuration.transferQueueFamilyRequired,
          configuration.graphicsQueueFamilyRequired,
          surface
      );

      VkCommandPoolCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      createInfo.queueFamilyIndex = familyIndices.transferFamily.value();
      createInfo.flags = 0; // Optional

      if (
          vkCreateCommandPool(device, &createInfo, nullptr, &commandPool) != VK_SUCCESS
          || commandPool == nullptr
          ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create transfer command pool!");
      }

      return commandPool;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkCommandPool Factory::createGraphicsCommandPool(
      VkPhysicalDevice& physicalDevice,
      Configuration& configuration,
      VkSurfaceKHR& surface,
      VkDevice& device
  ) {
    try {
      VkCommandPool commandPool = nullptr;

      QueueFamilyIndexInfo familyIndices = findQueueFamilies(
          physicalDevice,
          configuration.computeQueueFamilyRequired,
          configuration.transferQueueFamilyRequired,
          configuration.graphicsQueueFamilyRequired,
          surface
      );

      VkCommandPoolCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      createInfo.queueFamilyIndex = familyIndices.graphicsFamily.value();
      createInfo.flags = 0; // Optional

      if (
          vkCreateCommandPool(device, &createInfo, nullptr, &commandPool) != VK_SUCCESS
          || commandPool == nullptr
          ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create graphics command pool!");
      }

      return commandPool;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkCommandPool Factory::createPresentCommandPool(
      VkPhysicalDevice& physicalDevice,
      Configuration& configuration,
      VkSurfaceKHR& surface,
      VkDevice& device
  ) {
    try {
      VkCommandPool commandPool = nullptr;

      QueueFamilyIndexInfo familyIndices = findQueueFamilies(
          physicalDevice,
          configuration.computeQueueFamilyRequired,
          configuration.transferQueueFamilyRequired,
          configuration.graphicsQueueFamilyRequired,
          surface
      );

      VkCommandPoolCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      createInfo.queueFamilyIndex = familyIndices.presentFamily.value();
      createInfo.flags = 0; // Optional

      if (
          vkCreateCommandPool(device, &createInfo, nullptr, &commandPool) != VK_SUCCESS
          || commandPool == nullptr
          ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create present command pool!");
      }

      return commandPool;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkCommandBuffer Factory::createCommandBuffer(VkDevice& device, VkCommandPool& commandPool) {
    try {
      VkCommandBuffer commandBuffer = nullptr;

      std::vector<VkCommandBuffer> commandBuffers;
      commandBuffers.resize(1);

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.commandPool = commandPool;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

      if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to allocate command buffer!");
      }

      commandBuffer = commandBuffers[0];

      if (commandBuffer == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to create command buffer!");
      }

      return commandBuffer;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  std::vector<VkCommandBuffer> Factory::createCommandBuffers(
      VkDevice& device,
      VkCommandPool& commandPool,
      std::size_t size
  ) {
    try {
      std::vector<VkCommandBuffer> commandBuffers;
      commandBuffers.resize(size);

      VkCommandBufferAllocateInfo allocInfo = {};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.commandPool = commandPool;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

      if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to allocate command buffers!");
      }

      return commandBuffers;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  // destroy

  void Factory::destroyCommandBuffer(VkCommandBuffer& commandBuffer, VkCommandPool& commandPool, VkDevice& device) {
    try {
      if (commandBuffer != nullptr) {
        std::vector<VkCommandBuffer> commandBuffers = {commandBuffer};
        vkFreeCommandBuffers(device, commandPool, 1, commandBuffers.data());
        commandBuffer = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyCommandBuffers(std::vector<VkCommandBuffer>& commandBuffers, VkCommandPool& commandPool, VkDevice& device) {
    try {
      vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyCommandPool(VkCommandPool& commandPool, VkDevice& device) {
    try {
      if (commandPool != nullptr) {
        vkDestroyCommandPool(device, commandPool, nullptr);
        commandPool = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyDescriptorPool(VkDescriptorPool& descriptorPool, VkDevice& device) {
    try {
      if (descriptorPool != nullptr) {
        vkDestroyDescriptorPool(device, descriptorPool, nullptr);
        descriptorPool = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyFrameBuffer(VkFramebuffer& frameBuffer, VkDevice& device) {
    try {
      if (frameBuffer != nullptr) {
        vkDestroyFramebuffer(device, frameBuffer, nullptr);
        frameBuffer = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyFrameBuffers(std::vector<VkFramebuffer>& frameBuffers, VkDevice& device) {
    try {
      for (std::size_t i = 0; i < frameBuffers.size(); i++) {
        destroyFrameBuffer(frameBuffers[i], device);
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyPipeline(Pipeline& pipeline, VkDevice& device) {
    try {
      destroyPipeline(pipeline.value, device);
      destroyPipelineLayout(pipeline.layout, device);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyPipeline(VkPipeline& pipeline, VkDevice& device) {
    try {
      if (pipeline != nullptr) {
        vkDestroyPipeline(device, pipeline, nullptr);
        pipeline = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyPipelineLayout(VkPipelineLayout& pipelineLayout, VkDevice& device) {
    try {
      if (pipelineLayout != nullptr) {
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        pipelineLayout = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyShader(Shader& shader, VkDevice& device) {
    try {
      if (shader.shaderModule != nullptr) {
        vkDestroyShaderModule(device, shader.shaderModule, nullptr);
        shader.shaderModule = nullptr;
      }
      shader.pipelineShaderStageCreateInfo = {};
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyDescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout, VkDevice& device) {
    try {
      if (descriptorSetLayout != nullptr) {
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
        descriptorSetLayout = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyRenderPass(VkRenderPass& renderPass, VkDevice& device) {
    try {
      if (renderPass != nullptr) {
        vkDestroyRenderPass(device, renderPass, nullptr);
        renderPass = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyImageView(VkImageView& imageView, VkDevice& device) {
    try {
      if (imageView != nullptr) {
        vkDestroyImageView(device, imageView, nullptr);
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyImageViews(std::vector<VkImageView>& imageViews, VkDevice& device) {
    try {
      for (std::size_t i = 0; i < imageViews.size(); i++) {
        destroyImageView(imageViews[i], device);
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyImage(Image& image, VkDevice& device) {
    try {
      if (image.value != nullptr) {
        vkDestroyImage(device, image.value, nullptr);
        image.value = nullptr;
      }
      if (image.memory != nullptr) {
        vkFreeMemory(device, image.memory, nullptr);
        image.memory = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyImages(std::vector<Image>& images, VkDevice& device) {
    try {
      for (std::size_t i = 0; i < images.size(); i++) {
        destroyImage(images[i], device);
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyBuffer(Buffer& buffer, VkDevice& device) {
    try {
      if (buffer.value != nullptr) {
        vkDestroyBuffer(device, buffer.value, nullptr);
        buffer.value = nullptr;
      }
      if (buffer.memory != nullptr) {
        vkFreeMemory(device, buffer.memory, nullptr);
        buffer.memory = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroySwapChain(SwapChain& swapChain, VkDevice& device) {
    try {
      destroySwapChain(swapChain.value, device);
      swapChain.extent = {};
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroySwapChain(VkSwapchainKHR& swapChain, VkDevice& device) {
    try {
      if (swapChain != nullptr) {
        vkDestroySwapchainKHR(device, swapChain, nullptr);
        swapChain = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyQueue(VkQueue& queue) {
    try {
      queue = nullptr;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyDevice(VkDevice& device) {
    try {
      if (device != nullptr) {
        vkDestroyDevice(device, nullptr);
        device = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyPhysicalDevice(PhysicalDevice& physicalDevice) {
    try {
      physicalDevice.queueFamilyIndexInfo = {};
      physicalDevice.swapChainSupportDetails = {};
      physicalDevice.value = nullptr;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroySurface(VkSurfaceKHR& surface, VkInstance& instance) {
    try {
      if (surface != nullptr) {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        surface = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT& debugUtilsMessenger, VkInstance& instance) {
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

  void Factory::destroyInstance(VkInstance& instance) {
    try {
      if (instance != nullptr) {
        vkDestroyInstance(instance, nullptr);
        instance = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Factory::~Factory() = default;

}
