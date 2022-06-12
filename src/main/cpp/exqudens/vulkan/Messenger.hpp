#pragma once

#include <string>
#include <optional>
#include <functional>
#include <ostream>
#include <iostream>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct Messenger {

    static VkBool32 callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT cSeverity,
        VkDebugUtilsMessageTypeFlagsEXT cType,
        const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
        void* data
    ) {
      Messenger* messenger = nullptr;
      std::string formatted = "<UNDEFINED>";
      try {
        auto severity = vk::DebugUtilsMessageSeverityFlagsEXT(cSeverity);
        auto type = vk::DebugUtilsMessageTypeFlagsEXT(cType);
        std::string message(callbackData->pMessage);
        formatted = vk::to_string(severity) + " " + vk::to_string(type) + ": " + message;

        if (data != nullptr) {
          try {
            messenger = reinterpret_cast<Messenger*>(data);
          } catch (...) {
          }
          if (messenger != nullptr) {
            if (messenger->toStringFunction) {
              formatted = messenger->toStringFunction(severity, type, message);
            }
            if (messenger->exceptionSeverity.has_value() && severity >= messenger->exceptionSeverity.value()) {
              throw std::runtime_error(formatted);
            }
            if (messenger->outSeverity.has_value() && severity >= messenger->outSeverity.value()) {
              if (messenger->out != nullptr) {
                *messenger->out << formatted << std::endl;
                return VK_FALSE;
              } else {
                std::cout << formatted << std::endl;
                return VK_FALSE;
              }
            }
          }
        }

        return VK_FALSE;
      } catch (const std::exception& e) {
        if (messenger != nullptr && messenger->out != nullptr) {
          *messenger->out << formatted << std::endl;
        } else {
          std::cout << formatted << std::endl;
        }
        std::throw_with_nested(std::runtime_error(CALL_INFO() + ": " + e.what()));
      } catch (...) {
        if (messenger != nullptr && messenger->out != nullptr) {
          *messenger->out << formatted << std::endl;
        } else {
          std::cout << formatted << std::endl;
        }
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

    unsigned int id;
    std::optional<vk::DebugUtilsMessageSeverityFlagsEXT> exceptionSeverity;
    std::optional<vk::DebugUtilsMessageSeverityFlagsEXT> outSeverity;
    std::function<std::string(
        vk::DebugUtilsMessageSeverityFlagsEXT,
        vk::DebugUtilsMessageTypeFlagsEXT,
        std::string
    )> toStringFunction;
    std::ostream* out;

  };

}
