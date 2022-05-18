#pragma once

#include <cstdlib>
#include <string>
#include <optional>

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan {

  class Utility {

    public:

      Utility() = default;

      virtual void setEnvironmentVariable(const std::string& name, const std::string& value) {
        try {
#if defined(_WINDOWS)
          _putenv_s(name.c_str(), value.c_str());
#endif
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual std::optional<std::string> getEnvironmentVariable(const std::string& name) {
        try {
          std::optional<std::string> value;
#if defined(_WINDOWS)
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

      virtual std::string toString(
          vk::DebugUtilsMessageSeverityFlagsEXT severity,
          vk::DebugUtilsMessageTypeFlagsEXT type,
          const std::string& message
      ) {
        try {
          return vk::to_string(severity) + " " + vk::to_string(type) + ": " + message;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      /*virtual std::vector<uint32_t> getQueueFamilyIndices(
          const vk::raii::PhysicalDevice& physicalDevice,
          std::optional<vk::QueueFlagBits> type,
          const std::shared_ptr<vk::raii::SurfaceKHR>& surface
      ) {
        try {
          std::vector<uint32_t> queueFamilyIndices;

          std::vector<vk::QueueFamilyProperties> properties = physicalDevice.getQueueFamilyProperties();

          if (type.has_value()) {
            for (std::size_t i = 0; i < properties.size(); i++) {
              if (properties[i].queueFlags & *type) {
                auto queueFamilyIndex = static_cast<uint32_t>(i);
                queueFamilyIndices.emplace_back(queueFamilyIndex);
              }
            }
          }

          if (surface) {
            for (std::size_t i = 0; i < properties.size(); i++) {
              if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), *(*surface))) {
                auto queueFamilyIndex = static_cast<uint32_t>(i);
                queueFamilyIndices.emplace_back(queueFamilyIndex);
              }
            }
          }

          return queueFamilyIndices;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }*/

      /*virtual uint32_t getQueueFamilyCount(
          const vk::raii::PhysicalDevice& physicalDevice,
          const uint32_t& queueFamilyIndex
      ) {
        try {
          std::vector<vk::QueueFamilyProperties> properties = physicalDevice.getQueueFamilyProperties();
          auto i = static_cast<std::size_t>(queueFamilyIndex);
          return properties[i].queueCount;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }*/

      /*virtual bool isDeviceExtensionSupported(
          const vk::raii::PhysicalDevice& physicalDevice,
          const std::vector<const char*>& deviceExtensions
      ) {
        try {
          std::vector<vk::ExtensionProperties> properties = physicalDevice.enumerateDeviceExtensionProperties(nullptr);
          std::set<std::string> required(deviceExtensions.begin(), deviceExtensions.end());

          for (const auto& extension : properties) {
            std::string name = std::string(extension.extensionName.data());
            required.erase(name);
          }

          return required.empty();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }*/

  };

}
