#pragma once

#include <utility>

#include "exqudens/vulkan/Export.hpp"
#include "exqudens/vulkan/model/StringVector.hpp"

namespace exqudens::vulkan {

  struct EXQUDENS_VULKAN_EXPORT Configuration {

    bool validationLayersEnabled = false;
    StringVector validationLayers = StringVector();
    StringVector extensions = StringVector();
    StringVector deviceExtensions = StringVector();

    Configuration() = default;

    Configuration(
        bool validationLayersEnabled,
        StringVector validationLayers,
        StringVector extensions,
        StringVector deviceExtensions
    ):
        validationLayersEnabled(validationLayersEnabled),
        validationLayers(std::move(validationLayers)),
        extensions(std::move(extensions)),
        deviceExtensions(std::move(deviceExtensions))
    {
    }

    Configuration(const Configuration& object): Configuration(
        object.validationLayersEnabled,
        object.validationLayers,
        object.extensions,
        object.deviceExtensions
    ) {
    }

    Configuration(Configuration&& object) noexcept: Configuration() {
      swap(*this, object);
    }

    Configuration& operator=(Configuration object) {
      swap(*this, object);
      return *this;
    }

    friend void swap(Configuration& first, Configuration& second) {
      std::swap(first.validationLayersEnabled, second.validationLayersEnabled);
      std::swap(first.validationLayers, second.validationLayers);
      std::swap(first.extensions, second.extensions);
      std::swap(first.deviceExtensions, second.deviceExtensions);
    }

    virtual ~Configuration() = default;

  };

}
