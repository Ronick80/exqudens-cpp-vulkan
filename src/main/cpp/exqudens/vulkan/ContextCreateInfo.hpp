#pragma once

#include <vector>
#include <map>

namespace exqudens::vulkan {

  class ContextCreateInfo {

    public:

      std::map<const char*, const char*> environmentVariables;
      std::vector<const char*> enabledLayerNames;
      std::vector<const char*> enabledExtensionNames;
      std::vector<const char*> enabledDeviceExtensionNames;

      ContextCreateInfo& setEnvironmentVariables(const std::map<const char*, const char*>& value) {
        environmentVariables = value;
        return *this;
      }

      ContextCreateInfo& setEnabledLayerNames(const std::vector<const char*>& value) {
        enabledLayerNames = value;
        return *this;
      }

      ContextCreateInfo& setEnabledExtensionNames(const std::vector<const char*>& value) {
        enabledExtensionNames = value;
        return *this;
      }

      ContextCreateInfo& setEnabledDeviceExtensionNames(const std::vector<const char*>& value) {
        enabledDeviceExtensionNames = value;
        return *this;
      }

  };

}
