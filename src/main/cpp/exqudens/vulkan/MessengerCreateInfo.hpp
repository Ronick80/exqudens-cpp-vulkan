#pragma once

#include <string>
#include <optional>
#include <functional>
#include <ostream>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct MessengerCreateInfo {

    std::optional<vk::DebugUtilsMessageSeverityFlagsEXT> exceptionSeverity;
    std::optional<vk::DebugUtilsMessageSeverityFlagsEXT> outSeverity;
    std::function<std::string(
        vk::DebugUtilsMessageSeverityFlagsEXT,
        vk::DebugUtilsMessageTypeFlagsEXT,
        std::string
    )> toStringFunction;

    MessengerCreateInfo& setExceptionSeverity(const vk::DebugUtilsMessageSeverityFlagsEXT& value) {
      exceptionSeverity = value;
      return *this;
    }

    MessengerCreateInfo& setOutSeverity(const vk::DebugUtilsMessageSeverityFlagsEXT& value) {
      outSeverity = value;
      return *this;
    }

    MessengerCreateInfo& setToStringFunction(
        const std::function<std::string(
            vk::DebugUtilsMessageSeverityFlagsEXT,
            vk::DebugUtilsMessageTypeFlagsEXT,
            std::string
        )>& value
    ) {
      toStringFunction = value;
      return *this;
    }

  };

}
