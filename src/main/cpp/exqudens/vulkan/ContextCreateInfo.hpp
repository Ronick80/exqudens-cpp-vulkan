#pragma once

#include <vector>
#include <map>

namespace exqudens::vulkan {

  class ContextCreateInfo {

    public:

      std::map<const char*, const char*> environmentVariables;

      ContextCreateInfo& setEnvironmentVariables(const std::map<const char*, const char*>& value) {
        environmentVariables = value;
        return *this;
      }

  };

}
