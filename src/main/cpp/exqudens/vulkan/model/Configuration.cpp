#include "exqudens/vulkan/model/Configuration.hpp"

namespace exqudens::vulkan {

  Configuration::Configuration(
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

  Configuration::Configuration() = default;

  Configuration::Configuration(const Configuration& object): Configuration(
      object.validationLayersEnabled,
      object.validationLayers,
      object.extensions,
      object.deviceExtensions
  ) {
  }

  Configuration::Configuration(Configuration&& object) noexcept: Configuration() {
    swap(*this, object);
  }

  Configuration& Configuration::operator=(const Configuration& object) {
    return *this = Configuration(object);
  }

  Configuration& Configuration::operator=(Configuration&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(Configuration& first, Configuration& second) {
    std::swap(first.validationLayersEnabled, second.validationLayersEnabled);
    std::swap(first.validationLayers, second.validationLayers);
    std::swap(first.extensions, second.extensions);
    std::swap(first.deviceExtensions, second.deviceExtensions);
  }

  Configuration::~Configuration() = default;

}
