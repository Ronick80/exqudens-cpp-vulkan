#include "exqudens/vulkan/Logger.hpp"

#include <utility>

namespace exqudens::vulkan {

  VkBool32 Logger::call(
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

  Logger::Logger(
      std::function<void(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          std::string
      )> function
  ):
      function(std::move(function))
  {
  }

  Logger::Logger(const Logger& object): Logger(
      object.function
  ) {
  }

  Logger::Logger(Logger&& object) noexcept: Logger() {
    swap(*this, object);
  }

  Logger& Logger::operator=(Logger object) {
    swap(*this, object);
    return *this;
  }

  void swap(Logger& first, Logger& second) {
    std::swap(first.function, second.function);
  }
}
