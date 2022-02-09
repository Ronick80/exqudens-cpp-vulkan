#include "exqudens/vulkan/model/QueueFamilyIndices.hpp"

#include <utility>

namespace exqudens::vulkan {

  QueueFamilyIndices::QueueFamilyIndices(
      bool transferFamilyRequired,
      std::optional<uint32_t> transferFamily,

      bool graphicsFamilyRequired,
      std::optional<uint32_t> graphicsFamily,

      bool presentFamilyRequired,
      std::optional<uint32_t> presentFamily
  ):
      transferFamilyRequired(transferFamilyRequired),
      transferFamily(transferFamily),

      graphicsFamilyRequired(graphicsFamilyRequired),
      graphicsFamily(graphicsFamily),

      presentFamilyRequired(presentFamilyRequired),
      presentFamily(presentFamily)
  {
  }

  QueueFamilyIndices::QueueFamilyIndices() = default;

  QueueFamilyIndices::QueueFamilyIndices(const QueueFamilyIndices& object): QueueFamilyIndices(
      object.transferFamilyRequired,
      object.transferFamily,

      object.graphicsFamilyRequired,
      object.graphicsFamily,

      object.presentFamilyRequired,
      object.presentFamily
  ) {
  }

  QueueFamilyIndices::QueueFamilyIndices(QueueFamilyIndices&& object) noexcept: QueueFamilyIndices() {
    swap(*this, object);
  }

  QueueFamilyIndices& QueueFamilyIndices::operator=(const QueueFamilyIndices& object) {
    return *this = QueueFamilyIndices(object);
  }

  QueueFamilyIndices& QueueFamilyIndices::operator=(QueueFamilyIndices&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(QueueFamilyIndices& first, QueueFamilyIndices& second) {
    std::swap(first.transferFamilyRequired, second.transferFamilyRequired);
    std::swap(first.transferFamily, second.transferFamily);

    std::swap(first.graphicsFamilyRequired, second.graphicsFamilyRequired);
    std::swap(first.graphicsFamily, second.graphicsFamily);

    std::swap(first.presentFamilyRequired, second.presentFamilyRequired);
    std::swap(first.presentFamily, second.presentFamily);
  }

  bool QueueFamilyIndices::isComplete() {
    bool transferFamilyComplete = !transferFamilyRequired || transferFamily.has_value();
    bool graphicsFamilyComplete = !graphicsFamilyRequired || graphicsFamily.has_value();
    bool presentFamilyComplete = !presentFamilyRequired || presentFamily.has_value();
    return transferFamilyComplete && graphicsFamilyComplete && presentFamilyComplete;
  }

  QueueFamilyIndices::~QueueFamilyIndices() = default;

}
