#pragma once

#include <string>
#include <optional>
#include <vector>
#include <set>
#include <map>
#include <stdexcept>

#include "vulkan/vulkan_raii.hpp"

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan::raii {

  class Utility {

    public:

      static bool isValidationLayersSupported(
          const vk::raii::Context& context,
          const std::vector<const char*>& enabledLayerNames
      ) {
        try {
          uint32_t propertyCount = 0;
          vk::Result result = vk::enumerateInstanceLayerProperties(&propertyCount, nullptr, *context.getDispatcher());

          if (result != vk::Result::eSuccess) {
            throw std::runtime_error(CALL_INFO() + ": failed to enumerate instance layer properties count!");
          }

          std::vector<vk::LayerProperties> properties(propertyCount);
          result = vk::enumerateInstanceLayerProperties(&propertyCount, properties.data(), *context.getDispatcher());

          if (result != vk::Result::eSuccess) {
            throw std::runtime_error(CALL_INFO() + ": failed to enumerate instance layer properties!");
          }

          for (const char* enabledLayerName : enabledLayerNames) {
            bool layerFound = false;

            for (const vk::LayerProperties& layerProperties : properties) {
              std::string layerName = std::string(enabledLayerName);
              std::string name = std::string(layerProperties.layerName.data());
              if (layerName == name) {
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

      static bool isDeviceExtensionSupported(
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
      }

      static std::vector<uint32_t> getQueueFamilyIndices(
          const vk::raii::PhysicalDevice& physicalDevice,
          std::optional<vk::QueueFlagBits> type,
          const vk::raii::SurfaceKHR* surface
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

          if (surface != nullptr) {
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
      }

      static uint32_t getQueueFamilyCount(
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
      }

      static std::vector<std::size_t> getPhysicalDeviceIndices(
          const std::vector<vk::raii::PhysicalDevice>& physicalDevices,
          const std::set<vk::QueueFlagBits>& types,
          const vk::raii::SurfaceKHR* surface,
          const std::vector<const char*>& enabledDeviceExtensionNames,
          const bool& anisotropyRequired
      ) {
        try {
          std::vector<std::size_t> physicalDeviceIndices = {};

          for (std::size_t i = 0; i < physicalDevices.size(); i++) {

            bool queueFamilyIndicesAdequate = true;
            for (const vk::QueueFlagBits& type : types) {
              std::vector<uint32_t> indices = getQueueFamilyIndices(physicalDevices[i], type, surface);
              if (indices.empty()) {
                queueFamilyIndicesAdequate = false;
                break;
              }
            }

            bool deviceExtensionAdequate = isDeviceExtensionSupported(physicalDevices[i], enabledDeviceExtensionNames);

            bool swapChainAdequate = surface == nullptr;
            if (surface != nullptr) {
              std::vector<vk::SurfaceFormatKHR> formats = physicalDevices[i].getSurfaceFormatsKHR(*(*surface));
              std::vector<vk::PresentModeKHR> presentModes = physicalDevices[i].getSurfacePresentModesKHR(*(*surface));
              swapChainAdequate = !formats.empty() && !presentModes.empty();
            }

            bool anisotropyAdequate = !anisotropyRequired;
            if (anisotropyRequired) {
              vk::PhysicalDeviceFeatures features = physicalDevices[i].getFeatures();
              anisotropyAdequate = features.samplerAnisotropy;
            }

            if (
                queueFamilyIndicesAdequate
                && deviceExtensionAdequate
                && swapChainAdequate
                && anisotropyAdequate
            ) {
              physicalDeviceIndices.emplace_back(i);
            }
            
          }

          return physicalDeviceIndices;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
