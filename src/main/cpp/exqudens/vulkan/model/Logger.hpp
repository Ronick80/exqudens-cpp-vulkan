#pragma once

#include <string>
#include <functional>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Logger {

    static VkBool32 call(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
        void* userData
    ) {
      auto* logger = reinterpret_cast<Logger*>(userData);
      std::string message(callbackData->pMessage);
      logger->function(messageSeverity, messageType, message);
      return VK_FALSE;
    }

    std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const std::string&)> function;

  };

}
