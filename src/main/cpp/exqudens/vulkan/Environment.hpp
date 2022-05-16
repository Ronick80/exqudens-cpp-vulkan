#pragma once

#include <memory>
#include <map>
#include <utility>
#include <ostream>

#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/Context.hpp"
#include "exqudens/vulkan/Instance.hpp"
#include "exqudens/vulkan/Messenger.hpp"
#include "exqudens/vulkan/DebugUtilsMessenger.hpp"
#include "exqudens/vulkan/Surface.hpp"

namespace exqudens::vulkan {

  class Environment {

    protected:

      unsigned int contextId = 1;
      unsigned int instanceId = 1;
      unsigned int messengerId = 1;
      unsigned int debugUtilsMessengerId = 1;
      unsigned int surfaceId = 1;

      std::shared_ptr<Utility> utility = {};

      std::map<unsigned int, std::shared_ptr<Context>> contexts = {};
      std::map<unsigned int, std::shared_ptr<Instance>> instances = {};
      std::map<unsigned int, std::shared_ptr<Messenger>> messengers = {};
      std::map<unsigned int, std::shared_ptr<DebugUtilsMessenger>> debugUtilsMessengers = {};
      std::map<unsigned int, std::shared_ptr<Surface>> surfaces = {};

    public:

      explicit Environment(std::shared_ptr<Utility> utility): utility(std::move(utility)) {
      }

      Environment(): Environment(std::make_shared<Utility>()) {
      }

      std::shared_ptr<Context> createContext(
          const ContextCreateInfo& createInfo
      ) {
        try {
          for (auto const& [key, value] : createInfo.environmentVariables) {
            utility->setEnvironmentVariable(std::string(key), std::string(value));
          }

          auto* value = new Context;

          value->id = contextId++;
          value->createInfo = createInfo;

          auto* object = new vk::raii::Context;

          value->value = std::shared_ptr<vk::raii::Context>(object);

          return contexts[value->id] = std::shared_ptr<Context>(value);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::shared_ptr<Instance> createInstance(
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

          return instances[value->id] = std::shared_ptr<Instance>(value);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::shared_ptr<Messenger> createMessenger(
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
          value->toStringFunction = toStringFunction;
          value->out = &out;

          return messengers[value->id] = std::shared_ptr<Messenger>(value);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::shared_ptr<DebugUtilsMessenger> createDebugUtilsMessenger(
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

          return debugUtilsMessengers[value->id] = std::shared_ptr<DebugUtilsMessenger>(value);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::shared_ptr<Surface> createSurface(
          Instance& instance,
          VkSurfaceKHR& vkSurface
      ) {
        try {
          auto* value = new Surface;

          value->id = surfaceId++;

          auto* object = new vk::raii::SurfaceKHR(*instance.value, vkSurface);

          value->value = std::shared_ptr<vk::raii::SurfaceKHR>(object);

          return surfaces[value->id] = std::shared_ptr<Surface>(value);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
