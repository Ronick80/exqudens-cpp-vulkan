#include "exqudens/vulkan/Factory.hpp"
#include "exqudens/vulkan/Macros.hpp"

#include <array>
#include <set>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <cstdlib>

namespace exqudens::vulkan {

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

  std::function<std::string(
      VkDebugUtilsMessageSeverityFlagBitsEXT,
      VkDebugUtilsMessageTypeFlagsEXT,
      const std::string&
  )> Factory::createLoggerFunction() {
    try {
      return [](
          VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
          VkDebugUtilsMessageTypeFlagsEXT messageType,
          const std::string& message
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

        return line;
      };
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
      functions.enumerateInstanceLayerProperties(&layerCount, nullptr);

      std::vector<VkLayerProperties> availableLayers(layerCount);
      functions.enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

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
      const VkSurfaceKHR& surface
  ) {
    try {
      QueueFamilyIndexInfo result;
      result.computeFamilyRequired = computeFamilyRequired;
      result.transferFamilyRequired = transferFamilyRequired;
      result.graphicsFamilyRequired = graphicsFamilyRequired;
      result.presentFamilyRequired = surface != nullptr;

      uint32_t queueFamilyCount = 0;
      functions.getPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      functions.getPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

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
          functions.getPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
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
      functions.enumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

      std::vector<VkExtensionProperties> availableExtensions(extensionCount);
      functions.enumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

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

      functions.getPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

      uint32_t formatCount;
      functions.getPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

      if (formatCount != 0) {
        details.formats.resize(formatCount);
        functions.getPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
      }

      uint32_t presentModeCount;
      functions.getPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

      if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        functions.getPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
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

  VkExtent2D Factory::chooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities, const uint32_t& width, const uint32_t& height) {
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
      functions.getPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

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

  VkFormat Factory::findSupportedFormat(
      VkPhysicalDevice& physicalDevice,
      const std::vector<VkFormat>& candidates,
      VkImageTiling tiling,
      VkFormatFeatureFlags features
  ) {
    try {
      for (VkFormat format : candidates) {
        VkFormatProperties props;
        functions.getPhysicalDeviceFormatProperties(physicalDevice, format, &props);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
          return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
          return format;
        }
      }
      throw std::runtime_error(CALL_INFO() + ": failed to find supported format!");
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkFormat Factory::findDepthFormat(VkPhysicalDevice& physicalDevice) {
    try {
      return findSupportedFormat(
          physicalDevice,
          {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
          VK_IMAGE_TILING_OPTIMAL,
          VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
      );
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
      return {
          .applicationName = "Exqudens Application",
          .applicationVersionMajor = 1,
          .applicationVersionMinor = 0,
          .applicationVersionPatch = 0,
          .engineName = "Exqudens Engine",
          .engineVersionMajor = 1,
          .engineVersionMinor = 0,
          .engineVersionPatch = 0,
          .validationLayersEnabled = true,
          .validationLayers = {"VK_LAYER_KHRONOS_validation"},
          .extensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME},
          .deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
          .computeQueueFamilyRequired = true,
          .transferQueueFamilyRequired = true,
          .graphicsQueueFamilyRequired = true,
          .presentQueueFamilyRequired = true,
          .anisotropyRequired = true
      };
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Logger Factory::createLogger() {
    try {
      return createLogger(
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
          std::cout
      );
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Logger Factory::createLogger(std::ostream& stream) {
    try {
      return createLogger(
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
          stream
      );
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Logger Factory::createLogger(
      VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
      VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity
  ) {
    try {
      std::function<std::string(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          std::string
      )> function = loggerFunction;
      try {
        loggerFunction(
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
            "Test 'loggerFunction'."
        );
      } catch (const std::bad_function_call& e) {
        function = createLoggerFunction();
      }
      return createLogger(function, exceptionSeverity, outSeverity, std::cout);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Logger Factory::createLogger(
      VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
      VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity,
      std::ostream& stream
  ) {
    try {
      std::function<std::string(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          std::string
      )> function = loggerFunction;
      try {
        loggerFunction(
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
            "Test 'loggerFunction'."
        );
      } catch (const std::bad_function_call& e) {
        function = createLoggerFunction();
      }
      return createLogger(function, exceptionSeverity, outSeverity, stream);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Logger Factory::createLogger(
      const std::function<std::string(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          const std::string&
      )>& function,
      VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
      VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity
  ) {
    try {
      return createLogger(function, exceptionSeverity, outSeverity, std::cout);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Logger Factory::createLogger(
      const std::function<std::string(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          const std::string&
      )>& function,
      VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
      VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity,
      std::ostream& stream
  ) {
    try {
      return {
        .function = function,
        .exceptionSeverity = exceptionSeverity,
        .outSeverity = outSeverity,
        .stream = &stream
      };
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Functions Factory::createFunctions() {
    try {
      return {
        .enumerateInstanceLayerProperties = vkEnumerateInstanceLayerProperties,
        .enumerateDeviceExtensionProperties = vkEnumerateDeviceExtensionProperties,
        .enumeratePhysicalDevices = vkEnumeratePhysicalDevices,
        .getPhysicalDeviceQueueFamilyProperties = vkGetPhysicalDeviceQueueFamilyProperties,
        .getPhysicalDeviceSurfaceSupportKHR = vkGetPhysicalDeviceSurfaceSupportKHR,
        .getPhysicalDeviceFeatures = vkGetPhysicalDeviceFeatures,
        .getPhysicalDeviceSurfaceCapabilitiesKHR = vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
        .getPhysicalDeviceSurfaceFormatsKHR = vkGetPhysicalDeviceSurfaceFormatsKHR,
        .getPhysicalDeviceSurfacePresentModesKHR = vkGetPhysicalDeviceSurfacePresentModesKHR,
        .getPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties,
        .getPhysicalDeviceFormatProperties = vkGetPhysicalDeviceFormatProperties,
        .getInstanceProcAddr = vkGetInstanceProcAddr,
        .getDeviceQueue = vkGetDeviceQueue,
        .getBufferMemoryRequirements = vkGetBufferMemoryRequirements,
        .getImageMemoryRequirements = vkGetImageMemoryRequirements,
        .getSwapchainImagesKHR = vkGetSwapchainImagesKHR,
        .updateDescriptorSets = vkUpdateDescriptorSets,
        .createInstance = vkCreateInstance,
        .createDevice = vkCreateDevice,
        .createCommandPool = vkCreateCommandPool,
        .createSwapchainKHR = vkCreateSwapchainKHR,
        .createBuffer = vkCreateBuffer,
        .createImage = vkCreateImage,
        .createImageView = vkCreateImageView,
        .createRenderPass = vkCreateRenderPass,
        .createDescriptorSetLayout = vkCreateDescriptorSetLayout,
        .createShaderModule = vkCreateShaderModule,
        .createPipelineLayout = vkCreatePipelineLayout,
        .createGraphicsPipelines = vkCreateGraphicsPipelines,
        .createFramebuffer = vkCreateFramebuffer,
        .createSampler = vkCreateSampler,
        .createDescriptorPool = vkCreateDescriptorPool,
        .createSemaphore = vkCreateSemaphore,
        .createFence = vkCreateFence,
        .allocateMemory = vkAllocateMemory,
        .allocateDescriptorSets = vkAllocateDescriptorSets,
        .allocateCommandBuffers = vkAllocateCommandBuffers,
        .bindBufferMemory = vkBindBufferMemory,
        .bindImageMemory = vkBindImageMemory,
        .unmapMemory = vkUnmapMemory,
        .freeMemory = vkFreeMemory,
        .freeCommandBuffers = vkFreeCommandBuffers,
        .destroyFence = vkDestroyFence,
        .destroySemaphore = vkDestroySemaphore,
        .destroyDescriptorPool = vkDestroyDescriptorPool,
        .destroySampler = vkDestroySampler,
        .destroyFramebuffer = vkDestroyFramebuffer,
        .destroyPipeline = vkDestroyPipeline,
        .destroyShaderModule = vkDestroyShaderModule,
        .destroyDescriptorSetLayout = vkDestroyDescriptorSetLayout,
        .destroyRenderPass = vkDestroyRenderPass,
        .destroyImageView = vkDestroyImageView,
        .destroyImage = vkDestroyImage,
        .destroyBuffer = vkDestroyBuffer,
        .destroySwapchainKHR = vkDestroySwapchainKHR,
        .destroyCommandPool = vkDestroyCommandPool,
        .destroyDevice = vkDestroyDevice,
        .destroySurfaceKHR = vkDestroySurfaceKHR,
        .destroyInstance = vkDestroyInstance
      };
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

      if (functions.createInstance(&createInfo, nullptr, &instance) != VK_SUCCESS || instance == nullptr) {
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

      auto func = (PFN_vkCreateDebugUtilsMessengerEXT) functions.getInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

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

      if (createSurfaceFunction) {
        surface = createSurfaceFunction(instance);
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
      functions.enumeratePhysicalDevices(instance, &deviceCount, nullptr);

      if (deviceCount == 0) {
        throw std::runtime_error(CALL_INFO() + ": failed to find GPUs with Vulkan support!");
      }

      std::vector<VkPhysicalDevice> devices(deviceCount);
      functions.enumeratePhysicalDevices(instance, &deviceCount, devices.data());

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
        functions.getPhysicalDeviceFeatures(object, &supportedFeatures);

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

      if (
          functions.createDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS
          || device == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create logical device!");
      }

      return device;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Queue Factory::createQueue(
      VkDevice& device,
      uint32_t queueFamilyIndex,
      uint32_t queueIndex
  ) {
    try {
      VkQueue queue = nullptr;

      functions.getDeviceQueue(device, queueFamilyIndex, queueIndex, &queue);

      if (queue == nullptr) {
        throw std::runtime_error(CALL_INFO() + ": failed to create compute queue!");
      }

      return {
        .familyIndex = queueFamilyIndex,
        .value = queue
      };
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkCommandPool Factory::createCommandPool(
      VkDevice& device,
      uint32_t queueFamilyIndex
  ) {
    try {
      return createCommandPool(device, queueFamilyIndex, 0);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkCommandPool Factory::createCommandPool(
      VkDevice& device,
      uint32_t queueFamilyIndex,
      VkCommandPoolCreateFlags flags
  ) {
    try {
      VkCommandPool commandPool = nullptr;

      VkCommandPoolCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      createInfo.queueFamilyIndex = queueFamilyIndex;
      createInfo.flags = flags;

      if (
          functions.createCommandPool(device, &createInfo, nullptr, &commandPool) != VK_SUCCESS
          || commandPool == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create compute command pool!");
      }

      return commandPool;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  SwapChain Factory::createSwapChain(
      SwapChainSupportDetails& swapChainSupport,
      QueueFamilyIndexInfo& queueFamilyIndexInfo,
      VkSurfaceKHR& surface,
      VkDevice& device,
      const uint32_t& width,
      const uint32_t& height
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
        createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
      } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      }

      createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
      createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
      createInfo.presentMode = presentMode;
      createInfo.clipped = VK_TRUE;

      createInfo.oldSwapchain = nullptr;

      if (
          functions.createSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS
          || swapChain == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create swap chain!");
      }

      return {
        .format = surfaceFormat.format,
        .extent = extent,
        .imageCount = imageCount,
        .width = width,
        .height = height,
        .value = swapChain
      };
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Buffer Factory::createBuffer(
      VkPhysicalDevice& physicalDevice,
      VkDevice& device,
      const BufferCreateInfo& createInfo,
      VkMemoryPropertyFlags properties
  ) {
    try {
      VkBuffer buffer = nullptr;

      VkBufferCreateInfo bufferInfo = {
          .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
          .flags = createInfo.flags,
          .size = createInfo.size,
          .usage = createInfo.usage,
          .sharingMode = createInfo.sharingMode,
          .queueFamilyIndexCount = static_cast<uint32_t>(createInfo.queueFamilyIndices.size()),
          .pQueueFamilyIndices = createInfo.queueFamilyIndices.empty() ? nullptr : createInfo.queueFamilyIndices.data()
      };

      if (
          functions.createBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS
          || buffer == nullptr
          ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create buffer!");
      }

      VkDeviceMemory bufferMemory = nullptr;

      VkMemoryRequirements memRequirements;
      functions.getBufferMemoryRequirements(device, buffer, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

      if (
          functions.allocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS
          || bufferMemory == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to allocate buffer memory!");
      }

      if (functions.bindBufferMemory(device, buffer, bufferMemory, 0) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to bind buffer memory!");
      }

      return {
          .memory = bufferMemory,
          .memorySize = createInfo.size,
          .memoryProperties = properties,
          .value = buffer
      };
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Buffer Factory::createBuffer(
      VkPhysicalDevice& physicalDevice,
      VkDevice& device,
      VkDeviceSize memorySize,
      VkBufferUsageFlags usage,
      VkMemoryPropertyFlags properties
  ) {
    try {
      return createBuffer(
          physicalDevice,
          device,
          BufferCreateInfo {
              .flags = 0,
              .size = memorySize,
              .usage = usage,
              .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
              .queueFamilyIndices = {}
          },
          properties
      );
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  std::vector<Buffer> Factory::createBuffers(
      VkPhysicalDevice& physicalDevice,
      VkDevice& device,
      VkDeviceSize memorySize,
      VkBufferUsageFlags usage,
      VkMemoryPropertyFlags properties,
      std::size_t size
  ) {
    try {
      std::vector<Buffer> buffers;
      buffers.resize(size);
      for (std::size_t i = 0; i < size; i++) {
        buffers[i] = createBuffer(physicalDevice, device, memorySize, usage, properties);
      }
      return buffers;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Image Factory::createImage(
      VkPhysicalDevice& physicalDevice,
      VkDevice& device,
      const ImageCreateInfo& createInfo,
      VkMemoryPropertyFlags properties
  ) {
    try {
      VkImageCreateInfo imageInfo = {
          .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
          .flags = createInfo.flags,
          .imageType = createInfo.imageType,
          .format = createInfo.format,
          .extent = createInfo.extent,
          .mipLevels = createInfo.mipLevels,
          .arrayLayers = createInfo.arrayLayers,
          .samples = createInfo.samples,
          .tiling = createInfo.tiling,
          .usage = createInfo.usage,
          .sharingMode = createInfo.sharingMode,
          .queueFamilyIndexCount = static_cast<uint32_t>(createInfo.queueFamilyIndices.size()),
          .pQueueFamilyIndices = createInfo.queueFamilyIndices.empty() ? nullptr : createInfo.queueFamilyIndices.data(),
          .initialLayout = createInfo.initialLayout
      };

      VkImage image = nullptr;

      if (
          functions.createImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS
          || image == nullptr
          ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create image!");
      }

      VkDeviceMemory imageMemory = nullptr;

      VkMemoryRequirements memRequirements;
      functions.getImageMemoryRequirements(device, image, &memRequirements);

      VkMemoryAllocateInfo allocInfo = {};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

      if (functions.allocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
      }

      if (functions.bindImageMemory(device, image, imageMemory, 0) != VK_SUCCESS) {
        throw std::runtime_error("failed to bind image memory!");
      }

      return {
          .width = createInfo.extent.width,
          .height = createInfo.extent.height,
          .format = createInfo.format,
          .memory = imageMemory,
          .memorySize = memRequirements.size,
          .memoryProperties = properties,
          .value = image
      };
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
      return createImage(
          physicalDevice,
          device,
          ImageCreateInfo {
              .flags = 0,
              .imageType = VK_IMAGE_TYPE_2D,
              .format = format,
              .extent = VkExtent3D {
                  .width = width,
                  .height = height,
                  .depth = 1
              },
              .mipLevels = 1,
              .arrayLayers = 1,
              .samples = VK_SAMPLE_COUNT_1_BIT,
              .tiling = tiling,
              .usage = usage,
              .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
              .queueFamilyIndices = {},
              .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
          },
          properties
      );
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
      if (functions.getSwapchainImagesKHR(device, swapChain, &imageCount, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("failed to get swap chain image count!");
      }
      swapChainImages.resize(imageCount);
      if (functions.getSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to get swap chain images!");
      }
      return swapChainImages;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkImageView Factory::createImageView(VkDevice& device, VkImage& image, VkFormat format) {
    try {
      return createImageView(device, image, format, VK_IMAGE_ASPECT_COLOR_BIT);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkImageView Factory::createImageView(VkDevice& device, VkImage& image, VkFormat format, VkImageAspectFlags aspectMask) {
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
      viewInfo.subresourceRange.aspectMask = aspectMask;
      viewInfo.subresourceRange.baseMipLevel = 0;
      viewInfo.subresourceRange.levelCount = 1;
      viewInfo.subresourceRange.baseArrayLayer = 0;
      viewInfo.subresourceRange.layerCount = 1;

      VkImageView imageView = nullptr;

      if (
          functions.createImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS
          || imageView == nullptr
      ) {
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
      VkFormat format
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

  std::vector<VkImageView> Factory::createImageViews(
      VkDevice& device,
      std::vector<VkImage>& images,
      VkFormat format,
      VkImageAspectFlags aspectMask
  ) {
    try {
      std::vector<VkImageView> imageViews;

      imageViews.resize(images.size());
      for (uint32_t i = 0; i < images.size(); i++) {
        imageViews[i] = createImageView(device, images[i], format, aspectMask);
      }

      return imageViews;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkRenderPass Factory::createRenderPass(VkDevice& device, VkFormat& format) {
    try {
      return createRenderPass(
          device,
          RenderPassCreateInfo {
            .attachments = {
                VkAttachmentDescription {
                  .format = format,
                  .samples = VK_SAMPLE_COUNT_1_BIT,
                  .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                  .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                  .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                  .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                  .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                  .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
                }
            },
            .subPasses = {
                SubPassDescription {
                  .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                  .colorAttachments = {
                      VkAttachmentReference {
                        .attachment = 0,
                        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                      }
                  }
                }
            },
            .dependencies = {
                VkSubpassDependency {
                  .srcSubpass = VK_SUBPASS_EXTERNAL,
                  .dstSubpass = 0,
                  .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                  .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                  .srcAccessMask = 0,
                  .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
                }
            }
          }
      );
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkRenderPass Factory::createRenderPass(VkDevice& device, const RenderPassCreateInfo& createInfo) {
    try {
      VkRenderPass renderPass = nullptr;

      std::vector<VkSubpassDescription> subPasses;

      for (const SubPassDescription& subPassDescription : createInfo.subPasses) {
        VkSubpassDescription subPass = {
            .flags = subPassDescription.flags,
            .pipelineBindPoint = subPassDescription.pipelineBindPoint,
            .inputAttachmentCount = static_cast<uint32_t>(subPassDescription.inputAttachments.size()),
            .pInputAttachments = subPassDescription.inputAttachments.data(),
            .colorAttachmentCount = static_cast<uint32_t>(subPassDescription.colorAttachments.size()),
            .pColorAttachments = subPassDescription.colorAttachments.data(),
            .pResolveAttachments = subPassDescription.resolveAttachments.data(),
            .pDepthStencilAttachment = subPassDescription.depthStencilAttachment.has_value() ? &subPassDescription.depthStencilAttachment.value() : nullptr,
            .preserveAttachmentCount = static_cast<uint32_t>(subPassDescription.preserveAttachments.size()),
            .pPreserveAttachments = subPassDescription.preserveAttachments.data()
        };

        subPasses.emplace_back(subPass);
      }

      VkRenderPassCreateInfo info = {
          .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
          .pNext = nullptr,
          .flags = createInfo.flags,
          .attachmentCount = static_cast<uint32_t>(createInfo.attachments.size()),
          .pAttachments = createInfo.attachments.data(),
          .subpassCount = static_cast<uint32_t>(subPasses.size()),
          .pSubpasses = subPasses.data(),
          .dependencyCount = static_cast<uint32_t>(createInfo.dependencies.size()),
          .pDependencies = createInfo.dependencies.data()
      };

      if (
          functions.createRenderPass(device, &info, nullptr, &renderPass) != VK_SUCCESS
          || renderPass == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create render pass!");
      }

      return renderPass;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkDescriptorSetLayout Factory::createDescriptorSetLayout(VkDevice& device) {
    try {
      return createDescriptorSetLayout(
          device,
          DescriptorSetLayoutCreateInfo {
            .flags = 0,
            .bindings = {
                VkDescriptorSetLayoutBinding {
                  .binding = 0,
                  .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                  .descriptorCount = 1,
                  .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                  .pImmutableSamplers = nullptr
                },
                VkDescriptorSetLayoutBinding {
                  .binding = 1,
                  .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                  .descriptorCount = 1,
                  .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                  .pImmutableSamplers = nullptr
                }
            }
          }
      );
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkDescriptorSetLayout Factory::createDescriptorSetLayout(
      VkDevice& device,
      const DescriptorSetLayoutCreateInfo& createInfo
  ) {
    try {
      VkDescriptorSetLayout descriptorSetLayout = nullptr;

      VkDescriptorSetLayoutCreateInfo layoutInfo = {};
      layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      layoutInfo.flags = createInfo.flags;
      layoutInfo.bindingCount = static_cast<uint32_t>(createInfo.bindings.size());
      layoutInfo.pBindings = createInfo.bindings.data();

      if (
          functions.createDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS
          || descriptorSetLayout == nullptr
      ) {
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

      if (
          functions.createShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS
          || shaderModule == nullptr
      ) {
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

      return {
        .shaderModule = shaderModule,
        .pipelineShaderStageCreateInfo = pipelineShaderStageCreateInfo
      };
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Pipeline Factory::createPipeline(
      VkDevice& device,
      const std::vector<std::string>& shaderPaths,
      const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
      const std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions,
      const std::vector<VkVertexInputAttributeDescription>& vertexAttributeDescriptions,
      const PipelineLayoutCreateInfo& layoutCreateInfo,
      const GraphicsPipelineCreateInfo& createInfo
  ) {
    try {
      VkPipelineLayout pipelineLayout = nullptr;

      VkPipelineLayoutCreateInfo vkLayoutCreateInfo = {
          .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
          .flags = layoutCreateInfo.flags,
          .setLayoutCount = static_cast<uint32_t>(layoutCreateInfo.setLayouts.size()),
          .pSetLayouts = layoutCreateInfo.setLayouts.empty() ? nullptr : layoutCreateInfo.setLayouts.data(),
          .pushConstantRangeCount = static_cast<uint32_t>(layoutCreateInfo.pushConstantRanges.size()),
          .pPushConstantRanges = layoutCreateInfo.pushConstantRanges.empty() ? nullptr : layoutCreateInfo.pushConstantRanges.data()
      };

      if (
          functions.createPipelineLayout(device, &vkLayoutCreateInfo, nullptr, &pipelineLayout) != VK_SUCCESS
          || pipelineLayout == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create pipeline layout!");
      }

      VkPipeline pipeline = nullptr;

      VkGraphicsPipelineCreateInfo iCreateInfo = {};
      iCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

      std::vector<Shader> shaders;
      shaders.resize(shaderPaths.size());
      std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
      shaderStages.resize(shaders.size());
      for (std::size_t i = 0; i < shaderPaths.size(); i++) {
        shaders[i] = createShader(device, shaderPaths[i]);
        shaderStages[i] = shaders[i].pipelineShaderStageCreateInfo;
      }

      iCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
      iCreateInfo.pStages = shaderStages.data();

      VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
          .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
          .vertexBindingDescriptionCount = static_cast<uint32_t>(vertexBindingDescriptions.size()),
          .pVertexBindingDescriptions = vertexBindingDescriptions.empty() ? nullptr : vertexBindingDescriptions.data(),
          .vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributeDescriptions.size()),
          .pVertexAttributeDescriptions = vertexAttributeDescriptions.empty() ? nullptr : vertexAttributeDescriptions.data()
      };

      iCreateInfo.pVertexInputState = &vertexInputInfo;

      if (createInfo.inputAssemblyState.has_value()) {
        VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .flags = createInfo.inputAssemblyState.value().flags,
            .topology = createInfo.inputAssemblyState.value().topology,
            .primitiveRestartEnable = createInfo.inputAssemblyState.value().primitiveRestartEnable
        };

        iCreateInfo.pInputAssemblyState = &vkPipelineInputAssemblyStateCreateInfo;
      }

      if (createInfo.tessellationState.has_value()) {
        VkPipelineTessellationStateCreateInfo vkPipelineTessellationStateCreateInfo {
          .sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
          .flags = createInfo.tessellationState.value().flags,
          .patchControlPoints = createInfo.tessellationState.value().patchControlPoints
        };
        iCreateInfo.pTessellationState = &vkPipelineTessellationStateCreateInfo;
      }

      if (createInfo.viewportState.has_value()) {
        VkPipelineViewportStateCreateInfo viewportState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = static_cast<uint32_t>(createInfo.viewportState.value().viewports.size()),
            .pViewports = createInfo.viewportState.value().viewports.empty() ? nullptr : createInfo.viewportState.value().viewports.data(),
            .scissorCount = static_cast<uint32_t>(createInfo.viewportState.value().scissors.size()),
            .pScissors = createInfo.viewportState.value().scissors.empty() ? nullptr : createInfo.viewportState.value().scissors.data()
        };

        iCreateInfo.pViewportState = &viewportState;
      }

      if (createInfo.rasterizationState.has_value()) {
        VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .flags = createInfo.rasterizationState.value().flags,
            .depthClampEnable = createInfo.rasterizationState.value().depthClampEnable,
            .rasterizerDiscardEnable = createInfo.rasterizationState.value().rasterizerDiscardEnable,
            .polygonMode = createInfo.rasterizationState.value().polygonMode,
            .cullMode = createInfo.rasterizationState.value().cullMode,
            .frontFace = createInfo.rasterizationState.value().frontFace,
            .depthBiasEnable = createInfo.rasterizationState.value().depthBiasEnable,
            .depthBiasConstantFactor = createInfo.rasterizationState.value().depthBiasConstantFactor,
            .depthBiasClamp = createInfo.rasterizationState.value().depthBiasClamp,
            .depthBiasSlopeFactor = createInfo.rasterizationState.value().depthBiasSlopeFactor,
            .lineWidth = createInfo.rasterizationState.value().lineWidth
        };
        iCreateInfo.pRasterizationState = &vkPipelineRasterizationStateCreateInfo;
      }

      if (createInfo.multisampleState.has_value()) {
        VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .flags = createInfo.multisampleState.value().flags,
            .rasterizationSamples = createInfo.multisampleState.value().rasterizationSamples,
            .sampleShadingEnable = createInfo.multisampleState.value().sampleShadingEnable,
            .minSampleShading = createInfo.multisampleState.value().minSampleShading,
            .pSampleMask = createInfo.multisampleState.value().pSampleMask,
            .alphaToCoverageEnable = createInfo.multisampleState.value().alphaToCoverageEnable,
            .alphaToOneEnable = createInfo.multisampleState.value().alphaToOneEnable
        };
        iCreateInfo.pMultisampleState = &vkPipelineMultisampleStateCreateInfo;
      }

      if (createInfo.depthStencilState.has_value()) {
        VkPipelineDepthStencilStateCreateInfo vkPipelineDepthStencilStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .flags = createInfo.depthStencilState.value().flags,
            .depthTestEnable = createInfo.depthStencilState.value().depthTestEnable,
            .depthWriteEnable = createInfo.depthStencilState.value().depthWriteEnable,
            .depthCompareOp = createInfo.depthStencilState.value().depthCompareOp,
            .depthBoundsTestEnable = createInfo.depthStencilState.value().depthBoundsTestEnable,
            .stencilTestEnable = createInfo.depthStencilState.value().stencilTestEnable,
            .front = createInfo.depthStencilState.value().front,
            .back = createInfo.depthStencilState.value().back,
            .minDepthBounds = createInfo.depthStencilState.value().minDepthBounds,
            .maxDepthBounds = createInfo.depthStencilState.value().maxDepthBounds
        };
        iCreateInfo.pDepthStencilState = &vkPipelineDepthStencilStateCreateInfo;
      }

      if (createInfo.colorBlendState.has_value()) {
        VkPipelineColorBlendStateCreateInfo colorBlending = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .flags = createInfo.colorBlendState.value().flags,
            .logicOpEnable = createInfo.colorBlendState.value().logicOpEnable,
            .logicOp = createInfo.colorBlendState.value().logicOp,
            .attachmentCount = static_cast<uint32_t>(createInfo.colorBlendState.value().attachments.size()),
            .pAttachments = createInfo.colorBlendState.value().attachments.empty() ? nullptr : createInfo.colorBlendState.value().attachments.data()
        };
        colorBlending.blendConstants[0] = createInfo.colorBlendState.value().blendConstants[0];
        colorBlending.blendConstants[1] = createInfo.colorBlendState.value().blendConstants[1];
        colorBlending.blendConstants[2] = createInfo.colorBlendState.value().blendConstants[2];
        colorBlending.blendConstants[3] = createInfo.colorBlendState.value().blendConstants[3];

        iCreateInfo.pColorBlendState = &colorBlending;
      }

      if (createInfo.dynamicState.has_value()) {
        VkPipelineDynamicStateCreateInfo dynamicState {
          .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
          .flags = createInfo.dynamicState.value().flags,
          .dynamicStateCount = static_cast<uint32_t>(createInfo.dynamicState.value().dynamicStates.size()),
          .pDynamicStates = createInfo.dynamicState.value().dynamicStates.empty() ? nullptr : createInfo.dynamicState.value().dynamicStates.data()
        };
        iCreateInfo.pDynamicState = &dynamicState;
      }

      iCreateInfo.layout = pipelineLayout;
      iCreateInfo.renderPass = createInfo.renderPass;
      iCreateInfo.subpass = createInfo.subpass;
      iCreateInfo.basePipelineHandle = createInfo.basePipelineHandle;
      iCreateInfo.basePipelineIndex = createInfo.basePipelineIndex;

      if (
          functions.createGraphicsPipelines(device, VK_NULL_HANDLE, 1, &iCreateInfo, nullptr, &pipeline) != VK_SUCCESS
          || pipeline == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create graphics pipeline!");
      }

      for (Shader& shader : shaders) {
        destroyShader(shader, device);
      }

      return {
          .layout = pipelineLayout,
          .value = pipeline
      };
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Pipeline Factory::createGraphicsPipeline(
      VkDevice& device,
      const VkExtent2D& extent,
      const std::vector<std::string>& shaderPaths,
      VkRenderPass& renderPass
  ) {
    try {
      VkFrontFace frontFace = VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
      std::vector<VkDescriptorSetLayout> descriptorSetLayouts = {};
      std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions = {}; //{Vertex::getBindingDescription()};
      std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions = {}; //Vertex::getAttributeDescriptions();
      return createGraphicsPipeline(
          device,
          extent,
          shaderPaths,
          renderPass,
          frontFace,
          descriptorSetLayouts,
          vertexBindingDescriptions,
          vertexAttributeDescriptions
      );
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  Pipeline Factory::createGraphicsPipeline(
      VkDevice& device,
      const VkExtent2D& extent,
      const std::vector<std::string>& shaderPaths,
      VkRenderPass& renderPass,
      VkFrontFace frontFace,
      std::vector<VkDescriptorSetLayout> descriptorSetLayouts,
      std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions,
      std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions
  ) {
    try {
      return createPipeline(
          device,
          shaderPaths,
          descriptorSetLayouts,
          vertexBindingDescriptions,
          vertexAttributeDescriptions,
          PipelineLayoutCreateInfo {
              .flags = 0,
              .setLayouts = {descriptorSetLayouts},
              .pushConstantRanges = {}
          },
          GraphicsPipelineCreateInfo {
              .flags = 0,
              .inputAssemblyState = PipelineInputAssemblyStateCreateInfo {
                  .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                  .primitiveRestartEnable = VK_FALSE
              },
              .tessellationState = {},
              .viewportState = PipelineViewportStateCreateInfo {
                .flags = 0,
                .viewports = {
                    VkViewport {
                        .x = 0.0f,
                        .y = 0.0f,
                        .width = static_cast<float>(extent.width),
                        .height = static_cast<float>(extent.height),
                        .minDepth = 0.0f,
                        .maxDepth = 1.0f
                    }
                },
                .scissors = {
                    VkRect2D {
                        .offset = {0, 0},
                        .extent = extent
                    }
                }
              },
              .rasterizationState = PipelineRasterizationStateCreateInfo {
                  .depthClampEnable = VK_FALSE,
                  .rasterizerDiscardEnable = VK_FALSE,
                  .polygonMode = VK_POLYGON_MODE_FILL,
                  .cullMode = VK_CULL_MODE_BACK_BIT,
                  .frontFace = frontFace, // black screen WARNING!!!
                  .depthBiasEnable = VK_FALSE,
                  .depthBiasConstantFactor = 0.0f, // Optional
                  .depthBiasClamp = 0.0f, // Optional
                  .depthBiasSlopeFactor = 0.0f, // Optional
                  .lineWidth = 1.0f
              },
              .multisampleState = PipelineMultisampleStateCreateInfo {
                  .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
                  .sampleShadingEnable = VK_FALSE,
                  .minSampleShading = 1.0f, // Optional
                  .pSampleMask = nullptr, // Optional
                  .alphaToCoverageEnable = VK_FALSE, // Optional
                  .alphaToOneEnable = VK_FALSE // Optional
              },
              .depthStencilState = PipelineDepthStencilStateCreateInfo {
                  .depthTestEnable = VK_TRUE,
                  .depthWriteEnable = VK_TRUE,
                  .depthCompareOp = VK_COMPARE_OP_LESS,
                  .depthBoundsTestEnable = VK_FALSE,
                  .stencilTestEnable = VK_FALSE,
                  .front = {}, // Optional
                  .back = {}, // Optional
                  .minDepthBounds = 0.0f, // Optional
                  .maxDepthBounds = 1.0f // Optional
              },
              .colorBlendState = PipelineColorBlendStateCreateInfo {
                  .flags = 0,
                  .logicOpEnable = VK_FALSE,
                  .logicOp = VK_LOGIC_OP_COPY,
                  .attachments = {
                      VkPipelineColorBlendAttachmentState {
                          .blendEnable = VK_FALSE,
                          .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
                          .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
                          .colorBlendOp = VK_BLEND_OP_ADD,
                          .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                          .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                          .alphaBlendOp = VK_BLEND_OP_ADD,
                          .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
                      }
                  },
                  .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}
              },
              .dynamicState = {},
              .renderPass = renderPass,
              .subpass = 0,
              .basePipelineHandle = nullptr,
              .basePipelineIndex = 0
          }
      );
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkFramebuffer Factory::createFrameBuffer(VkDevice& device, const FrameBufferCreateInfo& createInfo) {
    try {
      VkFramebuffer frameBuffer = nullptr;

      VkFramebufferCreateInfo frameBufferInfo = {
          .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
          .flags = createInfo.flags,
          .renderPass = createInfo.renderPass,
          .attachmentCount = static_cast<uint32_t>(createInfo.attachments.size()),
          .pAttachments = createInfo.attachments.data(),
          .width = createInfo.width,
          .height = createInfo.height,
          .layers = createInfo.layers
      };

      if (
          functions.createFramebuffer(device, &frameBufferInfo, nullptr, &frameBuffer) != VK_SUCCESS
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
      const std::vector<FrameBufferCreateInfo>& createInfo
  ) {
    try {
      std::vector<VkFramebuffer> frameBuffers;
      frameBuffers.resize(createInfo.size());
      for (std::size_t i = 0; i < frameBuffers.size(); i++) {
        frameBuffers[i] = createFrameBuffer(device, createInfo[i]);
      }
      return frameBuffers;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkSampler Factory::createSampler(VkPhysicalDevice& physicalDevice, VkDevice& device) {
    try {
      VkSampler sampler = nullptr;

      VkPhysicalDeviceProperties properties = {};
      vkGetPhysicalDeviceProperties(physicalDevice, &properties);

      VkSamplerCreateInfo samplerInfo{};
      samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      samplerInfo.magFilter = VK_FILTER_LINEAR;
      samplerInfo.minFilter = VK_FILTER_LINEAR;
      samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerInfo.anisotropyEnable = VK_TRUE;
      samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
      samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
      samplerInfo.unnormalizedCoordinates = VK_FALSE;
      samplerInfo.compareEnable = VK_FALSE;
      samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
      samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

      if (
          functions.createSampler(device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS
          || sampler == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create sampler!");
      }

      return sampler;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkDescriptorPool Factory::createDescriptorPool(VkDevice& device, uint32_t maxSets) {
    try {
      return createDescriptorPool(
          device,
          DescriptorPoolCreateInfo {
            .maxSets = maxSets,
            .poolSizes = {
                VkDescriptorPoolSize {
                  .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                  .descriptorCount = maxSets
                },
                VkDescriptorPoolSize {
                    .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    .descriptorCount = maxSets
                }
            }
          }
      );
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkDescriptorPool Factory::createDescriptorPool(VkDevice& device, const DescriptorPoolCreateInfo& createInfo) {
    try {
      VkDescriptorPool descriptorPool = nullptr;

      VkDescriptorPoolCreateInfo poolInfo = {};
      poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      poolInfo.poolSizeCount = static_cast<uint32_t>(createInfo.poolSizes.size());
      poolInfo.pPoolSizes = createInfo.poolSizes.data();
      poolInfo.maxSets = createInfo.maxSets;
      poolInfo.flags = createInfo.flags;

      if (
          functions.createDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS
          || descriptorPool == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create descriptor pool!");
      }

      return descriptorPool;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkDescriptorSet Factory::createDescriptorSet(
      VkDevice& device,
      VkDescriptorPool& descriptorPool,
      VkDescriptorSetLayout& descriptorSetLayout,
      const std::vector<WriteDescriptorSet>& writeDescriptorSets
  ) {
    try {
      VkDescriptorSet descriptorSet = nullptr;

      VkDescriptorSetAllocateInfo allocInfo = {};
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = descriptorPool;
      allocInfo.descriptorSetCount = descriptorSetLayout == nullptr ? 0 : 1;
      allocInfo.pSetLayouts = descriptorSetLayout == nullptr ? nullptr : &descriptorSetLayout;

      if (
          functions.allocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS
          || descriptorSet == nullptr
      ) {
        throw std::runtime_error(CALL_INFO()+ ": failed to allocate descriptor sets!");
      }

      std::vector<VkWriteDescriptorSet> writes;
      writes.resize(writeDescriptorSets.size());

      for (std::size_t i = 0; i < writes.size(); i++) {
        writes[i] = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = descriptorSet,
            .dstBinding = writeDescriptorSets[i].dstBinding,
            .dstArrayElement = writeDescriptorSets[i].dstArrayElement,
            .descriptorCount = writeDescriptorSets[i].descriptorCount,
            .descriptorType = writeDescriptorSets[i].descriptorType,
            .pImageInfo = writeDescriptorSets[i].imageInfo.empty() ? nullptr : writeDescriptorSets[i].imageInfo.data(),
            .pBufferInfo = writeDescriptorSets[i].bufferInfo.empty() ? nullptr : writeDescriptorSets[i].bufferInfo.data(),
            .pTexelBufferView = writeDescriptorSets[i].texelBufferView.empty() ? nullptr : writeDescriptorSets[i].texelBufferView.data()
        };
      }

      functions.updateDescriptorSets(device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);

      return descriptorSet;
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

      if (functions.allocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
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

      if (functions.allocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to allocate command buffers!");
      }

      return commandBuffers;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkSemaphore Factory::createSemaphore(VkDevice& device) {
    try {
      return createSemaphore(device, 0);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkSemaphore Factory::createSemaphore(VkDevice& device, VkSemaphoreCreateFlags flags) {
    try {
      VkSemaphore semaphore = nullptr;

      VkSemaphoreCreateInfo semaphoreInfo = {};
      semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
      semaphoreInfo.flags = flags;

      if (
          functions.createSemaphore(device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS
          || semaphore == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create semaphore!");
      }

      return semaphore;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  std::vector<VkSemaphore> Factory::createSemaphores(VkDevice& device, std::size_t size) {
    try {
      std::vector<VkSemaphore> semaphores;
      semaphores.resize(size);
      for (std::size_t i = 0; i < size; i++) {
        semaphores[i] = createSemaphore(device);
      }
      return semaphores;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  std::vector<VkSemaphore> Factory::createSemaphores(VkDevice& device, VkSemaphoreCreateFlags flags, std::size_t size) {
    try {
      std::vector<VkSemaphore> semaphores;
      semaphores.resize(size);
      for (std::size_t i = 0; i < size; i++) {
        semaphores[i] = createSemaphore(device, flags);
      }
      return semaphores;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkFence Factory::createFence(VkDevice& device) {
    try {
      return createFence(device, VK_FENCE_CREATE_SIGNALED_BIT);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  VkFence Factory::createFence(VkDevice& device, VkFenceCreateFlags flags) {
    try {
      VkFence fence = nullptr;

      VkFenceCreateInfo fenceInfo{};
      fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      fenceInfo.flags = flags;

      if (
          functions.createFence(device, &fenceInfo, nullptr, &fence) != VK_SUCCESS
          || fence == nullptr
      ) {
        throw std::runtime_error(CALL_INFO() + ": failed to create fence!");
      }

      return fence;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  std::vector<VkFence> Factory::createFences(VkDevice& device, std::size_t size) {
    try {
      std::vector<VkFence> fences;
      fences.resize(size);
      for (std::size_t i = 0; i < size; i++) {
        fences[i] = createFence(device);
      }
      return fences;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  std::vector<VkFence> Factory::createFences(VkDevice& device, VkFenceCreateFlags flags, std::size_t size) {
    try {
      std::vector<VkFence> fences;
      fences.resize(size);
      for (std::size_t i = 0; i < size; i++) {
        fences[i] = createFence(device, flags);
      }
      return fences;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  // destroy

  void Factory::destroyFence(VkFence& fence, VkDevice& device) {
    try {
      if (fence != nullptr) {
        functions.destroyFence(device, fence, nullptr);
        fence = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyFences(std::vector<VkFence>& fences, VkDevice& device) {
    try {
      for (std::size_t i = 0; i < fences.size(); i++) {
        destroyFence(fences[i], device);
      }
      fences.clear();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroySemaphore(VkSemaphore& semaphore, VkDevice& device) {
    try {
      if (semaphore != nullptr) {
        functions.destroySemaphore(device, semaphore, nullptr);
        semaphore = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroySemaphores(std::vector<VkSemaphore>& semaphores, VkDevice& device) {
    try {
      for (std::size_t i = 0; i < semaphores.size(); i++) {
        destroySemaphore(semaphores[i], device);
      }
      semaphores.clear();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyCommandBuffer(VkCommandBuffer& commandBuffer, VkCommandPool& commandPool, VkDevice& device) {
    try {
      if (commandBuffer != nullptr) {
        std::vector<VkCommandBuffer> commandBuffers = {commandBuffer};
        functions.freeCommandBuffers(device, commandPool, 1, commandBuffers.data());
        commandBuffer = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyCommandBuffers(std::vector<VkCommandBuffer>& commandBuffers, VkCommandPool& commandPool, VkDevice& device) {
    try {
      functions.freeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
      commandBuffers.clear();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyDescriptorSet(VkDescriptorSet& descriptorSet) {
    try {
      if (descriptorSet != nullptr) {
        descriptorSet = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyDescriptorSets(std::vector<VkDescriptorSet>& descriptorSets) {
    try {
      for (std::size_t i = 0; i < descriptorSets.size(); i++) {
        descriptorSets[i] = nullptr;
      }
      descriptorSets.clear();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyDescriptorPool(VkDescriptorPool& descriptorPool, VkDevice& device) {
    try {
      if (descriptorPool != nullptr) {
        functions.destroyDescriptorPool(device, descriptorPool, nullptr);
        descriptorPool = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroySampler(VkSampler& sampler, VkDevice& device) {
    try {
      if (sampler != nullptr) {
        functions.destroySampler(device, sampler, nullptr);
        sampler = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyFrameBuffer(VkFramebuffer& frameBuffer, VkDevice& device) {
    try {
      if (frameBuffer != nullptr) {
        functions.destroyFramebuffer(device, frameBuffer, nullptr);
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
      frameBuffers.clear();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyPipeline(Pipeline& pipeline, VkDevice& device) {
    try {
      if (pipeline.value != nullptr) {
        functions.destroyPipeline(device, pipeline.value, nullptr);
        pipeline.value = nullptr;
      }
      if (pipeline.layout != nullptr) {
        vkDestroyPipelineLayout(device, pipeline.layout, nullptr);
        pipeline.layout = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyShader(Shader& shader, VkDevice& device) {
    try {
      if (shader.shaderModule != nullptr) {
        functions.destroyShaderModule(device, shader.shaderModule, nullptr);
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
        functions.destroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
        descriptorSetLayout = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyRenderPass(VkRenderPass& renderPass, VkDevice& device) {
    try {
      if (renderPass != nullptr) {
        functions.destroyRenderPass(device, renderPass, nullptr);
        renderPass = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyImageView(VkImageView& imageView, VkDevice& device) {
    try {
      if (imageView != nullptr) {
        functions.destroyImageView(device, imageView, nullptr);
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
      imageViews.clear();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyImage(Image& image, VkDevice& device) {
    try {
      destroyImage(image, device, false);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyImage(Image& image, VkDevice& device, bool unmapMemory) {
    try {
      if (image.memory != nullptr) {
        if (
            unmapMemory
            && (image.memoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        ) {
          functions.unmapMemory(device, image.memory);
        }
        functions.freeMemory(device, image.memory, nullptr);
        image.memory = nullptr;
      }
      if (image.value != nullptr) {
        functions.destroyImage(device, image.value, nullptr);
        image.value = nullptr;
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
      images.clear();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyImages(std::vector<Image>& images, VkDevice& device, bool unmapMemory) {
    try {
      for (std::size_t i = 0; i < images.size(); i++) {
        destroyImage(images[i], device, unmapMemory);
      }
      images.clear();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyBuffer(Buffer& buffer, VkDevice& device) {
    try {
      destroyBuffer(buffer, device, false);
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyBuffer(Buffer& buffer, VkDevice& device, bool unmapMemory) {
    try {
      if (buffer.memory != nullptr) {
        if (
            unmapMemory
            && (buffer.memoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            ) {
          functions.unmapMemory(device, buffer.memory);
        }
        functions.freeMemory(device, buffer.memory, nullptr);
        buffer.memory = nullptr;
      }
      if (buffer.value != nullptr) {
        functions.destroyBuffer(device, buffer.value, nullptr);
        buffer.value = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyBuffers(std::vector<Buffer>& buffers, VkDevice& device) {
    try {
      for (std::size_t i = 0; i < buffers.size(); i++) {
        destroyBuffer(buffers[i], device);
      }
      buffers.clear();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyBuffers(std::vector<Buffer>& buffers, VkDevice& device, bool unmapMemory) {
    try {
      for (std::size_t i = 0; i < buffers.size(); i++) {
        destroyBuffer(buffers[i], device, unmapMemory);
      }
      buffers.clear();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroySwapChain(SwapChain& swapChain, VkDevice& device) {
    try {
      if (swapChain.value != nullptr) {
        functions.destroySwapchainKHR(device, swapChain.value, nullptr);
        swapChain.value = nullptr;
      }
      swapChain.extent = {};
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyCommandPool(VkCommandPool& commandPool, VkDevice& device) {
    try {
      if (commandPool != nullptr) {
        functions.destroyCommandPool(device, commandPool, nullptr);
        commandPool = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyQueue(Queue& queue) {
    try {
      queue.familyIndex = 0;
      queue.value = nullptr;
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyDevice(VkDevice& device) {
    try {
      if (device != nullptr) {
        functions.destroyDevice(device, nullptr);
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
        functions.destroySurfaceKHR(instance, surface, nullptr);
        surface = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void Factory::destroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT& debugUtilsMessenger, VkInstance& instance) {
    try {
      if (debugUtilsMessenger != nullptr) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) functions.getInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
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
        functions.destroyInstance(instance, nullptr);
        instance = nullptr;
      }
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

}
