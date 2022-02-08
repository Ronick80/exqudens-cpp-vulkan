#include "exqudens/vulkan/model/Configuration.hpp"

#include <utility>

namespace exqudens::vulkan {

  Configuration::Configuration(
      std::string applicationName,
      unsigned int applicationVersionMajor,
      unsigned int applicationVersionMinor,
      unsigned int applicationVersionPatch,
      std::string engineName,
      unsigned int engineVersionMajor,
      unsigned int engineVersionMinor,
      unsigned int engineVersionPatch,
      bool validationLayersEnabled,
      StringVector validationLayers,
      StringVector extensions,
      StringVector deviceExtensions
  ):
      applicationName(std::move(applicationName)),
      applicationVersionMajor(applicationVersionMajor),
      applicationVersionMinor(applicationVersionMinor),
      applicationVersionPatch(applicationVersionPatch),
      engineName(std::move(engineName)),
      engineVersionMajor(engineVersionMajor),
      engineVersionMinor(engineVersionMinor),
      engineVersionPatch(engineVersionPatch),
      validationLayersEnabled(validationLayersEnabled),
      validationLayers(std::move(validationLayers)),
      extensions(std::move(extensions)),
      deviceExtensions(std::move(deviceExtensions))
  {
  }

  Configuration::Configuration() = default;

  Configuration::Configuration(const Configuration& object): Configuration(
      object.applicationName,
      object.applicationVersionMajor,
      object.applicationVersionMinor,
      object.applicationVersionPatch,
      object.engineName,
      object.engineVersionMajor,
      object.engineVersionMinor,
      object.engineVersionPatch,
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
    std::swap(first.applicationName, second.applicationName);
    std::swap(first.applicationVersionMajor, second.applicationVersionMajor);
    std::swap(first.applicationVersionMinor, second.applicationVersionMinor);
    std::swap(first.applicationVersionPatch, second.applicationVersionPatch);
    std::swap(first.engineName, second.engineName);
    std::swap(first.engineVersionMajor, second.engineVersionMajor);
    std::swap(first.engineVersionMinor, second.engineVersionMinor);
    std::swap(first.engineVersionPatch, second.engineVersionPatch);
    std::swap(first.validationLayersEnabled, second.validationLayersEnabled);
    std::swap(first.validationLayers, second.validationLayers);
    std::swap(first.extensions, second.extensions);
    std::swap(first.deviceExtensions, second.deviceExtensions);
  }

  Configuration::~Configuration() = default;

}
