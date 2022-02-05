#pragma once

#include <string>
#include <functional>

#include "vulkan/vulkan.h"

namespace exqudens::vulkan {

  class Logger {

    private:

      std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)> function;

    public:

      static VkBool32 call(
          VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
          VkDebugUtilsMessageTypeFlagsEXT messageType,
          const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
          void* userData
      );

      explicit Logger(
          std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)> function
      );
      Logger() = default;
      Logger(const Logger& object);
      Logger(Logger&& object) noexcept;
      Logger& operator=(Logger object);
      friend void swap(Logger& first, Logger& second);
      virtual ~Logger() = default;

  };

}
