#pragma once

#include <string>
#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"
#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/MessengerCreateInfo.hpp"
#include "exqudens/vulkan/Messenger.hpp"

namespace exqudens::vulkan {

  struct Instance {

    class Builder;

    inline static Builder builder();

    std::shared_ptr<vk::raii::Context> context;
    std::vector<const char*> enabledLayerNames;
    std::vector<const char*> enabledExtensionNames;
    vk::ApplicationInfo applicationInfo;
    vk::InstanceCreateInfo createInfo;
    std::shared_ptr<vk::raii::Instance> value;
    MessengerCreateInfo messengerCreateInfo;
    std::shared_ptr<Messenger> messenger;
    vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo;
    std::shared_ptr<vk::raii::DebugUtilsMessengerEXT> debugUtilsMessenger;

    vk::raii::Context& contextReference() {
      try {
        if (!context) {
          throw std::runtime_error(CALL_INFO() + ": context is not initialized!");
        }
        return *context;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

    vk::raii::Instance& reference() {
      try {
        if (!value) {
          throw std::runtime_error(CALL_INFO() + ": value is not initialized!");
        }
        return *value;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

    Messenger& messengerReference() {
      try {
        if (!messenger) {
          throw std::runtime_error(CALL_INFO() + ": messenger is not initialized!");
        }
        return *messenger;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

    vk::raii::DebugUtilsMessengerEXT& debugUtilsMessengerReference() {
      try {
        if (!debugUtilsMessenger) {
          throw std::runtime_error(CALL_INFO() + ": debugUtilsMessenger is not initialized!");
        }
        return *debugUtilsMessenger;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

  };

  class Instance::Builder {

    private:

      std::vector<const char*> enabledLayerNames;
      std::vector<const char*> enabledExtensionNames;
      std::optional<vk::ApplicationInfo> applicationInfo;
      std::optional<vk::InstanceCreateInfo> createInfo;
      std::optional<MessengerCreateInfo> messengerCreateInfo;
      std::optional<vk::DebugUtilsMessengerCreateInfoEXT> debugUtilsMessengerCreateInfo;
      std::ostream* out;

    public:

      Instance::Builder& addEnabledLayerName(const char* val) {
        enabledLayerNames.emplace_back(val);
        return *this;
      }

      Instance::Builder& addEnabledLayerName(const std::string& val) {
        enabledLayerNames.emplace_back(val.c_str());
        return *this;
      }

      Instance::Builder& setEnabledLayerNames(const std::vector<const char*>& val) {
        enabledLayerNames = val;
        return *this;
      }

      Instance::Builder& addEnabledExtensionName(const char* val) {
        enabledExtensionNames.emplace_back(val);
        return *this;
      }

      Instance::Builder& addEnabledExtensionName(const std::string& val) {
        enabledExtensionNames.emplace_back(val.c_str());
        return *this;
      }

      Instance::Builder& setEnabledExtensionNames(const std::vector<const char*>& val) {
        enabledExtensionNames = val;
        return *this;
      }

      Instance::Builder& setApplicationInfo(const vk::ApplicationInfo& val) {
        applicationInfo = val;
        return *this;
      }

      Instance::Builder& setCreateInfo(const vk::InstanceCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      Instance::Builder& setMessengerCreateInfo(const MessengerCreateInfo& val) {
        messengerCreateInfo = val;
        return *this;
      }

      Instance::Builder& setDebugUtilsMessengerCreateInfo(const vk::DebugUtilsMessengerCreateInfoEXT& val) {
        debugUtilsMessengerCreateInfo = val;
        return *this;
      }

      Instance::Builder& setOut(std::ostream& val) {
        out = &val;
        return *this;
      }

      Instance build() {
        try {
          Instance target = {};
          target.context = std::make_shared<vk::raii::Context>();
          target.enabledLayerNames = enabledLayerNames;
          target.enabledExtensionNames = enabledExtensionNames;
          target.applicationInfo = applicationInfo.value();
          target.createInfo = createInfo.value_or(vk::InstanceCreateInfo());
          target.createInfo.setPEnabledLayerNames(target.enabledLayerNames);
          target.createInfo.setPEnabledExtensionNames(target.enabledExtensionNames);
          target.createInfo.setPApplicationInfo(&target.applicationInfo);
          target.value = std::make_shared<vk::raii::Instance>(
              target.contextReference(),
              target.createInfo
          );
          target.messengerCreateInfo = messengerCreateInfo.value();
          if (!target.messengerCreateInfo.toStringFunction) {
            /*target.messengerCreateInfo.toStringFunction = std::bind(
                &Environment::toString,
                this,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3
            );*/
            /*target.messengerCreateInfo.toStringFunction = [this](auto&& o1, auto&& o2, auto&& o3) {
              return toString(
                  std::forward<decltype(o1)>(o1),
                  std::forward<decltype(o2)>(o2),
                  std::forward<decltype(o3)>(o3)
              );
            };*/
            target.messengerCreateInfo.toStringFunction = &Utility::toString;
          }
          target.messenger = std::make_shared<Messenger>(
              *out,
              target.messengerCreateInfo
          );
          target.debugUtilsMessengerCreateInfo = debugUtilsMessengerCreateInfo.value();
          target.debugUtilsMessengerCreateInfo.setPUserData(&target.messengerReference());
          target.debugUtilsMessengerCreateInfo.setPfnUserCallback(&Messenger::callback);
          target.debugUtilsMessenger = std::make_shared<vk::raii::DebugUtilsMessengerEXT>(
              target.reference(),
              target.debugUtilsMessengerCreateInfo
          );
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  Instance::Builder Instance::builder() {
    return {};
  }

}
