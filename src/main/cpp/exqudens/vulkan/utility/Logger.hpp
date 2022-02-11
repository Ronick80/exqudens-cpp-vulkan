#pragma once

#include <string>
#include <functional>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Logger {

    public:

      static VkBool32 call(
          VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
          VkDebugUtilsMessageTypeFlagsEXT messageType,
          const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
          void* userData
      );

      std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const std::string&)> function;

      explicit Logger(
          std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const std::string&)> function
      );
      Logger() = default;
      Logger(const Logger& object);
      Logger(Logger&& object) noexcept;
      Logger& operator=(Logger object);
      friend void swap(Logger& first, Logger& second);
      virtual ~Logger() = default;

  };

}
