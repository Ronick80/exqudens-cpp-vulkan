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

namespace exqudens::vulkan {

  class Environment {

    protected:

      unsigned int contextId = 1;
      unsigned int instanceId = 1;
      unsigned int messengerId = 1;
      unsigned int debugUtilsMessengerId = 1;

      std::shared_ptr<Utility> utility = {};

      std::map<unsigned int, std::shared_ptr<Context>> contexts = {};
      std::map<unsigned int, std::shared_ptr<Instance>> instances = {};
      std::map<unsigned int, std::shared_ptr<Messenger>> messengers = {};
      std::map<unsigned int, std::shared_ptr<DebugUtilsMessenger>> debugUtilsMessengers = {};

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

          auto* context = new Context;

          context->id = contextId++;
          context->createInfo = createInfo;

          auto* value = new vk::raii::Context;

          context->value = std::shared_ptr<vk::raii::Context>(value);

          return contexts[context->id] = std::shared_ptr<Context>(context);
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
          auto* instance = new Instance;

          instance->id = instanceId++;
          instance->applicationInfo = applicationInfo;
          instance->createInfo = createInfo;

          instance->createInfo.pApplicationInfo = &instance->applicationInfo;

          auto* value = new vk::raii::Instance(
              *context.value,
              instance->createInfo
          );

          instance->value = std::shared_ptr<vk::raii::Instance>(value);

          return instances[instance->id] = std::shared_ptr<Instance>(instance);
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
          auto* messenger = new Messenger;

          messenger->id = messengerId++;
          messenger->exceptionSeverity = exceptionSeverity;
          messenger->outSeverity = outSeverity;
          messenger->toStringFunction = toStringFunction;
          messenger->out = &out;

          return messengers[messengerId++] = std::shared_ptr<Messenger>(messenger);
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
          auto* debugUtilsMessenger = new DebugUtilsMessenger;

          debugUtilsMessenger->id = debugUtilsMessengerId++;
          debugUtilsMessenger->createInfo = createInfo;

          auto* value = new vk::raii::DebugUtilsMessengerEXT(
              *instance.value,
              createInfo
          );

          debugUtilsMessenger->value = std::shared_ptr<vk::raii::DebugUtilsMessengerEXT>(value);

          return debugUtilsMessengers[debugUtilsMessenger->id] = std::shared_ptr<DebugUtilsMessenger>(debugUtilsMessenger);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
