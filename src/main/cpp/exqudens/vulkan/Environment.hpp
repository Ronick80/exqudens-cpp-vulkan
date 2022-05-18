#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <utility>
#include <ostream>

#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/Context.hpp"
#include "exqudens/vulkan/Instance.hpp"
#include "exqudens/vulkan/Messenger.hpp"
#include "exqudens/vulkan/DebugUtilsMessenger.hpp"
#include "exqudens/vulkan/Surface.hpp"
#include "exqudens/vulkan/PhysicalDevice.hpp"

namespace exqudens::vulkan {

  class Environment {

    protected:

      unsigned int contextId = 1;
      unsigned int instanceId = 1;
      unsigned int messengerId = 1;
      unsigned int debugUtilsMessengerId = 1;
      unsigned int surfaceId = 1;
      unsigned int physicalDeviceId = 1;

      std::vector<vk::raii::PhysicalDevice> physicalDevices = {};

      std::map<unsigned int, std::shared_ptr<Context>> contextMap = {};
      std::map<unsigned int, std::shared_ptr<Instance>> instanceMap = {};
      std::map<unsigned int, std::shared_ptr<Messenger>> messengerMap = {};
      std::map<unsigned int, std::shared_ptr<DebugUtilsMessenger>> debugUtilsMessengerMap = {};
      std::map<unsigned int, std::shared_ptr<Surface>> surfaceMap = {};
      std::map<unsigned int, std::shared_ptr<PhysicalDevice>> physicalDeviceMap = {};

    public:

      virtual std::shared_ptr<Context> createContext(
          const ContextCreateInfo& createInfo
      ) {
        try {
          for (auto const& [key, value] : createInfo.environmentVariables) {
            getUtility()->setEnvironmentVariable(std::string(key), std::string(value));
          }

          auto* value = new Context;
          value->id = contextId++;
          value->createInfo = createInfo;

          auto* object = new vk::raii::Context;

          value->value = std::shared_ptr<vk::raii::Context>(object);

          return contextMap[value->id] = std::shared_ptr<Context>(value);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual std::shared_ptr<Instance> createInstance(
          Context& context,
          const vk::ApplicationInfo& applicationInfo,
          const vk::InstanceCreateInfo& createInfo
      ) {
        try {
          auto* value = new Instance;
          value->id = instanceId++;
          value->applicationInfo = applicationInfo;
          value->createInfo = createInfo;
          value->createInfo.pApplicationInfo = &value->applicationInfo;
          auto* object = new vk::raii::Instance(
              *context.value,
              value->createInfo
          );
          value->value = std::shared_ptr<vk::raii::Instance>(object);
          return instanceMap[value->id] = std::shared_ptr<Instance>(value);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual std::shared_ptr<Messenger> createMessenger(
          std::ostream& out,
          const std::optional<vk::DebugUtilsMessageSeverityFlagsEXT>& exceptionSeverity,
          const std::optional<vk::DebugUtilsMessageSeverityFlagsEXT>& outSeverity,
          const std::function<std::string(
              vk::DebugUtilsMessageSeverityFlagsEXT,
              vk::DebugUtilsMessageTypeFlagsEXT,
              std::string
          )>& toStringFunction
      ) {
        try {
          auto* value = new Messenger;
          value->id = messengerId++;
          value->exceptionSeverity = exceptionSeverity;
          value->outSeverity = outSeverity;
          if (toStringFunction) {
            value->toStringFunction = toStringFunction;
          } else {
            /*value->toStringFunction = std::bind(
                &Utility::toString,
                utility,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3
            );*/
            value->toStringFunction = [this](auto&& o1, auto&& o2, auto&& o3) {
              return getUtility()->toString(
                  std::forward<decltype(o1)>(o1),
                  std::forward<decltype(o2)>(o2),
                  std::forward<decltype(o3)>(o3)
              );
            };
          }
          value->out = &out;
          return messengerMap[value->id] = std::shared_ptr<Messenger>(value);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual std::shared_ptr<DebugUtilsMessenger> createDebugUtilsMessenger(
          Instance& instance,
          Messenger& messenger,
          const vk::DebugUtilsMessengerCreateInfoEXT& createInfo
      ) {
        try {
          auto* value = new DebugUtilsMessenger;
          value->id = debugUtilsMessengerId++;
          value->createInfo = createInfo;
          auto* object = new vk::raii::DebugUtilsMessengerEXT(
              *instance.value,
              createInfo
          );
          value->value = std::shared_ptr<vk::raii::DebugUtilsMessengerEXT>(object);
          return debugUtilsMessengerMap[value->id] = std::shared_ptr<DebugUtilsMessenger>(value);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual std::shared_ptr<Surface> createSurface(
          Instance& instance,
          VkSurfaceKHR& vkSurface
      ) {
        try {
          auto* value = new Surface;
          value->id = surfaceId++;
          auto* object = new vk::raii::SurfaceKHR(*instance.value, vkSurface);
          value->value = std::shared_ptr<vk::raii::SurfaceKHR>(object);
          return surfaceMap[value->id] = std::shared_ptr<Surface>(value);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual std::shared_ptr<PhysicalDevice> createPhysicalDevice(
          Instance& instance,
          const std::vector<const char*>& enabledDeviceExtensionNames,
          const vk::PhysicalDeviceFeatures& features,
          const std::vector<vk::QueueFlagBits>& queueTypes,
          const std::shared_ptr<Surface>& surface
      ) {
        try {
          auto* value = new PhysicalDevice;
          value->id = physicalDeviceId++;
          value->features = features;
          physicalDevices = vk::raii::PhysicalDevices(*instance.value);
          for (vk::raii::PhysicalDevice& physicalDevice : physicalDevices) {

            bool queueFamilyIndicesAdequate = true;
            bool deviceExtensionAdequate = true;
            bool swapChainAdequate = true;
            bool anisotropyAdequate = true;

            std::vector<Queue> computeQueues = {};
            std::vector<Queue> transferQueues = {};
            std::vector<Queue> graphicsQueues = {};
            std::vector<Queue> presentQueues = {};

            std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
            for (const vk::QueueFlagBits& queueType : queueTypes) {
              std::vector<Queue> queues = {};
              for (std::size_t i = 0; i < queueFamilyProperties.size(); i++) {
                if (queueFamilyProperties[i].queueFlags & queueType) {
                  auto queueFamilyIndex = static_cast<uint32_t>(i);
                  uint32_t queueCount = queueFamilyProperties[i].queueCount;
                  queues.emplace_back(Queue {.familyIndex = queueFamilyIndex, .count = queueCount});
                }
              }
              if (queues.empty()) {
                queueFamilyIndicesAdequate = false;
                break;
              } else {
                if (vk::QueueFlagBits::eCompute == queueType) {
                  computeQueues = queues;
                } else if (vk::QueueFlagBits::eTransfer == queueType) {
                  transferQueues = queues;
                } else if (vk::QueueFlagBits::eGraphics == queueType) {
                  graphicsQueues = queues;
                }
              }
            }
            if (surface) {
              std::vector<Queue> queues = {};
              for (std::size_t i = 0; i < queueFamilyProperties.size(); i++) {
                if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), *(*surface->value))) {
                  auto queueFamilyIndex = static_cast<uint32_t>(i);
                  uint32_t queueCount = queueFamilyProperties[i].queueCount;
                  queues.emplace_back(Queue {.familyIndex = queueFamilyIndex, .count = queueCount});
                }
              }
              if (queues.empty()) {
                presentQueues = queues;
                queueFamilyIndicesAdequate = false;
              }
            }
            if (!queueFamilyIndicesAdequate) {
              break;
            }

            std::vector<vk::ExtensionProperties> extensionProperties = physicalDevice.enumerateDeviceExtensionProperties(nullptr);
            std::set<std::string> requiredExtensions(enabledDeviceExtensionNames.begin(), enabledDeviceExtensionNames.end());
            for (const auto& extension : extensionProperties) {
              std::string name = std::string(extension.extensionName.data());
              requiredExtensions.erase(name);
            }
            deviceExtensionAdequate = requiredExtensions.empty();
            if (!deviceExtensionAdequate) {
              break;
            }

            if (surface) {
              std::vector<vk::SurfaceFormatKHR> formats = physicalDevice.getSurfaceFormatsKHR(*(*surface->value));
              std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(*(*surface->value));
              swapChainAdequate = !formats.empty() && !presentModes.empty();
            }
            if (!swapChainAdequate) {
              break;
            }

            if (features.samplerAnisotropy) {
              vk::PhysicalDeviceFeatures physicalDeviceFeatures = physicalDevice.getFeatures();
              anisotropyAdequate = physicalDeviceFeatures.samplerAnisotropy;
            }
            if (!anisotropyAdequate) {
              break;
            }

            if (
                queueFamilyIndicesAdequate
                && deviceExtensionAdequate
                && swapChainAdequate
                && anisotropyAdequate
            ) {
              value->computeQueues = computeQueues;
              value->transferQueues = transferQueues;
              value->graphicsQueues = graphicsQueues;
              value->presentQueues = presentQueues;
              value->value = &physicalDevice;
              break;
            }
          }
          return physicalDeviceMap[value->id] = std::shared_ptr<PhysicalDevice>(value);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

    protected:

      virtual std::shared_ptr<Utility> getUtility() {
        try {
          return std::make_shared<Utility>();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
