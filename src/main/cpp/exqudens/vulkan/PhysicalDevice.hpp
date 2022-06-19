#pragma once

#include <optional>
#include <vector>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"
#include "exqudens/vulkan/PhysicalDeviceCreateInfo.hpp"

namespace exqudens::vulkan {

  struct PhysicalDevice {

    class Builder;

    static Builder builder();

    PhysicalDeviceCreateInfo createInfo;
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
      std::optional<PhysicalDeviceCreateInfo> createInfo;

    public:

      PhysicalDevice::Builder& setInstance(const std::weak_ptr<vk::raii::Instance>& val) {
        instance = val;
        return *this;
      }

      PhysicalDevice::Builder& setCreateInfo(const PhysicalDeviceCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      PhysicalDevice build() {
        try {
          PhysicalDevice target = {};
          target.createInfo = createInfo.value();
          if (target.createInfo.queuePriority > 1) {
            throw std::runtime_error(CALL_INFO() + ": target.createInfo.queuePriority > 1");
          }
          std::vector<vk::raii::PhysicalDevice> values = vk::raii::PhysicalDevices(
              *instance.lock()
          );
          for (vk::raii::PhysicalDevice& physicalDevice : values) {

            bool queueFamilyIndicesAdequate = true;
            bool deviceExtensionAdequate = true;
            bool swapChainAdequate = true;
            bool anisotropyAdequate = true;

            std::vector<vk::DeviceQueueCreateInfo> tmpComputeQueueCreateInfos = {};
            std::vector<vk::DeviceQueueCreateInfo> tmpTransferQueueCreateInfos = {};
            std::vector<vk::DeviceQueueCreateInfo> tmpGraphicsQueueCreateInfos = {};
            std::vector<vk::DeviceQueueCreateInfo> tmpPresentQueueCreateInfos = {};
            std::map<uint32_t, vk::DeviceQueueCreateInfo> queueCreateInfoMap = {};

            std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
            for (const vk::QueueFlagBits& queueType : target.createInfo.queueTypes) {
              std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = {};
              for (size_t i = 0; i < queueFamilyProperties.size(); i++) {
                if (queueFamilyProperties[i].queueFlags & queueType) {
                  vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo()
                      .setQueueFamilyIndex(static_cast<uint32_t>(i))
                      .setQueueCount(1)
                      .setPQueuePriorities(&target.createInfo.queuePriority);
                  queueCreateInfos.emplace_back(queueCreateInfo);
                  queueCreateInfoMap.try_emplace(queueCreateInfo.queueFamilyIndex, queueCreateInfo);
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
            if (target.createInfo.surface) {
              std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = {};
              for (std::size_t i = 0; i < queueFamilyProperties.size(); i++) {
                if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), target.createInfo.surface.value())) {
                  vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo()
                      .setQueueFamilyIndex(static_cast<uint32_t>(i))
                      .setQueueCount(1)
                      .setPQueuePriorities(&target.createInfo.queuePriority);
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
                target.createInfo.enabledExtensionNames.begin(),
                target.createInfo.enabledExtensionNames.end()
            );
            for (const auto& extension : extensionProperties) {
              std::string name = std::string(extension.extensionName.data());
              requiredExtensions.erase(name);
            }
            deviceExtensionAdequate = requiredExtensions.empty();
            if (!deviceExtensionAdequate) {
              continue;
            }

            if (target.createInfo.surface) {
              std::vector<vk::SurfaceFormatKHR> formats = physicalDevice.getSurfaceFormatsKHR(target.createInfo.surface.value());
              std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(target.createInfo.surface.value());
              swapChainAdequate = !formats.empty() && !presentModes.empty();
            }
            if (!swapChainAdequate) {
              continue;
            }

            if (target.createInfo.features.samplerAnisotropy) {
              vk::PhysicalDeviceFeatures physicalDeviceFeatures = physicalDevice.getFeatures();
              anisotropyAdequate = physicalDeviceFeatures.samplerAnisotropy;
            }
            if (!anisotropyAdequate) {
              continue;
            }

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
