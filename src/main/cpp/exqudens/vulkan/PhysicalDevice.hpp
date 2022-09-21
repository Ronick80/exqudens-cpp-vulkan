#pragma once

#include <string>
#include <optional>
#include <vector>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"
#include "exqudens/vulkan/QueueRequirement.hpp"

namespace exqudens::vulkan {

  struct PhysicalDevice {

    class Builder;

    inline static Builder builder();

    std::vector<const char*> enabledExtensionNames;
    vk::PhysicalDeviceFeatures features;
    std::optional<vk::PhysicalDeviceHostQueryResetFeatures> hostQueryResetFeatures;
    std::vector<QueueRequirement> queueRequirements;
    std::vector<float> queuePriorities;
    std::vector<vk::DeviceQueueCreateInfo> computeQueueCreateInfos;
    std::vector<vk::DeviceQueueCreateInfo> transferQueueCreateInfos;
    std::vector<vk::DeviceQueueCreateInfo> graphicsQueueCreateInfos;
    std::vector<vk::DeviceQueueCreateInfo> presentQueueCreateInfos;
    std::vector<vk::DeviceQueueCreateInfo> uniqueQueueCreateInfos;
    std::shared_ptr<vk::raii::PhysicalDevice> value;

    vk::raii::PhysicalDevice& reference() {
      try {
        if (!value) {
          throw std::runtime_error(CALL_INFO() + ": value is not initialized!");
        }
        return *value;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

  };

  class PhysicalDevice::Builder {

    private:

      std::weak_ptr<vk::raii::Instance> instance;
      std::vector<const char*> enabledExtensionNames;
      std::optional<vk::PhysicalDeviceFeatures> features;
      std::vector<QueueRequirement> queueRequirements;
      std::optional<vk::SurfaceKHR> surface;
      std::vector<float> queuePriorities;

    public:

      PhysicalDevice::Builder& setInstance(const std::weak_ptr<vk::raii::Instance>& val) {
        instance = val;
        return *this;
      }

      PhysicalDevice::Builder& addEnabledExtensionName(const char* val) {
        enabledExtensionNames.emplace_back(val);
        return *this;
      }

      PhysicalDevice::Builder& addEnabledExtensionName(const std::string& val) {
        enabledExtensionNames.emplace_back(val.c_str());
        return *this;
      }

      PhysicalDevice::Builder& setEnabledExtensionNames(const std::vector<const char*>& val) {
        enabledExtensionNames = val;
        return *this;
      }

      PhysicalDevice::Builder& setFeatures(const vk::PhysicalDeviceFeatures& val) {
        features = val;
        return *this;
      }

      PhysicalDevice::Builder& addQueueRequirement(const vk::QueueFlagBits& type) {
        queueRequirements.emplace_back(QueueRequirement().setType(type).setTimestampRequired(false));
        return *this;
      }

      PhysicalDevice::Builder& addQueueRequirement(const vk::QueueFlagBits& type, const bool& timestampRequired) {
        queueRequirements.emplace_back(QueueRequirement().setType(type).setTimestampRequired(timestampRequired));
        return *this;
      }

      PhysicalDevice::Builder& addQueueRequirement(const QueueRequirement& val) {
        queueRequirements.emplace_back(val);
        return *this;
      }

      PhysicalDevice::Builder& setQueueRequirements(const std::vector<QueueRequirement>& val) {
        queueRequirements = val;
        return *this;
      }

      PhysicalDevice::Builder& setSurface(const vk::SurfaceKHR& val) {
        surface = val;
        return *this;
      }

      PhysicalDevice::Builder& setSurface(const std::weak_ptr<vk::raii::SurfaceKHR>& val) {
        surface = *(*val.lock());
        return *this;
      }

      PhysicalDevice::Builder& setQueuePriority(const float& val) {
        queuePriorities.clear();
        queuePriorities.emplace_back(float(val));
        return *this;
      }

      PhysicalDevice build() {
        try {
          PhysicalDevice target = {};
          target.enabledExtensionNames = enabledExtensionNames;
          target.features = features.value_or(vk::PhysicalDeviceFeatures());
          target.queueRequirements = queueRequirements;
          target.queuePriorities = queuePriorities;
          std::vector<vk::raii::PhysicalDevice> values = vk::raii::PhysicalDevices(
              *instance.lock()
          );
          for (vk::raii::PhysicalDevice& physicalDevice : values) {

            bool queueFamilyIndicesAdequate = true;
            bool deviceExtensionAdequate = true;
            bool swapChainAdequate = true;
            bool anisotropyAdequate = true;

            std::optional<vk::PhysicalDeviceHostQueryResetFeatures> tmpHostQueryResetFeatures = {};
            std::vector<vk::DeviceQueueCreateInfo> tmpComputeQueueCreateInfos = {};
            std::vector<vk::DeviceQueueCreateInfo> tmpTransferQueueCreateInfos = {};
            std::vector<vk::DeviceQueueCreateInfo> tmpGraphicsQueueCreateInfos = {};
            std::vector<vk::DeviceQueueCreateInfo> tmpPresentQueueCreateInfos = {};
            std::map<uint32_t, vk::DeviceQueueCreateInfo> queueCreateInfoMap = {};

            std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
            for (const QueueRequirement& queueRequirement : target.queueRequirements) {
              vk::QueueFlagBits queueType = queueRequirement.type;
              bool queueTimestampRequired = queueRequirement.timestampRequired;

              std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = {};
              for (size_t i = 0; i < queueFamilyProperties.size(); i++) {

                bool queueTypeAdequate = (bool) (queueFamilyProperties[i].queueFlags & queueType);
                bool queueTimestampAdequate = !queueTimestampRequired || queueFamilyProperties[i].timestampValidBits > 0;

                if (queueTypeAdequate && queueTimestampAdequate) {
                  vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo()
                      .setQueueFamilyIndex(static_cast<uint32_t>(i))
                      .setQueuePriorities(target.queuePriorities);
                  queueCreateInfos.emplace_back(queueCreateInfo);
                  queueCreateInfoMap.try_emplace(queueCreateInfo.queueFamilyIndex, queueCreateInfo);
                  if (queueTimestampRequired) {
                    tmpHostQueryResetFeatures = vk::PhysicalDeviceHostQueryResetFeatures().setHostQueryReset(true);
                  }
                }
              }
              if (queueCreateInfos.empty()) {
                queueFamilyIndicesAdequate = false;
                break;
              } else {
                if (vk::QueueFlagBits::eCompute == queueType) {
                  tmpComputeQueueCreateInfos = queueCreateInfos;
                } else if (vk::QueueFlagBits::eTransfer == queueType) {
                  tmpTransferQueueCreateInfos = queueCreateInfos;
                } else if (vk::QueueFlagBits::eGraphics == queueType) {
                  tmpGraphicsQueueCreateInfos = queueCreateInfos;
                }
              }
            }
            if (!queueFamilyIndicesAdequate) {
              continue;
            }
            if (surface) {
              std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = {};
              for (std::size_t i = 0; i < queueFamilyProperties.size(); i++) {
                if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface.value())) {
                  vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo()
                      .setQueueFamilyIndex(static_cast<uint32_t>(i))
                      .setQueuePriorities(target.queuePriorities);
                  queueCreateInfos.emplace_back(queueCreateInfo);
                  queueCreateInfoMap.try_emplace(queueCreateInfo.queueFamilyIndex, queueCreateInfo);
                }
              }
              if (queueCreateInfos.empty()) {
                queueFamilyIndicesAdequate = false;
              } else {
                tmpPresentQueueCreateInfos = queueCreateInfos;
              }
            }
            if (!queueFamilyIndicesAdequate) {
              continue;
            }

            std::vector<vk::ExtensionProperties> extensionProperties = physicalDevice.enumerateDeviceExtensionProperties(nullptr);
            std::set<std::string> requiredExtensions(
                target.enabledExtensionNames.begin(),
                target.enabledExtensionNames.end()
            );
            for (const auto& extension : extensionProperties) {
              std::string name = std::string(extension.extensionName.data());
              requiredExtensions.erase(name);
            }
            deviceExtensionAdequate = requiredExtensions.empty();
            if (!deviceExtensionAdequate) {
              continue;
            }

            if (surface) {
              std::vector<vk::SurfaceFormatKHR> formats = physicalDevice.getSurfaceFormatsKHR(surface.value());
              std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(surface.value());
              swapChainAdequate = !formats.empty() && !presentModes.empty();
            }
            if (!swapChainAdequate) {
              continue;
            }

            if (target.features.samplerAnisotropy) {
              vk::PhysicalDeviceFeatures physicalDeviceFeatures = physicalDevice.getFeatures();
              anisotropyAdequate = physicalDeviceFeatures.samplerAnisotropy;
            }
            if (!anisotropyAdequate) {
              continue;
            }

            target.hostQueryResetFeatures = tmpHostQueryResetFeatures;
            target.computeQueueCreateInfos = tmpComputeQueueCreateInfos;
            target.transferQueueCreateInfos = tmpTransferQueueCreateInfos;
            target.graphicsQueueCreateInfos = tmpGraphicsQueueCreateInfos;
            target.presentQueueCreateInfos = tmpPresentQueueCreateInfos;
            std::vector<vk::DeviceQueueCreateInfo> tmpUniqueQueueCreateInfos;
            for (const auto& [k, v] : queueCreateInfoMap) {
              tmpUniqueQueueCreateInfos.emplace_back(v);
            }
            target.uniqueQueueCreateInfos = tmpUniqueQueueCreateInfos;
            target.value = std::make_shared<vk::raii::PhysicalDevice>(std::move(physicalDevice));
            break;
          }
          if (!target.value) {
            throw std::runtime_error(CALL_INFO() + ": failed to create physical device!");
          }
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  PhysicalDevice::Builder PhysicalDevice::builder() {
    return {};
  }

}
