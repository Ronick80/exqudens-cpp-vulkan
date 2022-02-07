#include "exqudens/vulkan/model/QueueFamilyIndices.hpp"

#include <utility>

namespace exqudens::vulkan {

  QueueFamilyIndices::QueueFamilyIndices(
      std::optional<uint32_t> transferFamily,
      std::optional<uint32_t> graphicsFamily,
      std::optional<uint32_t> presentFamily
  ):
      transferFamily(transferFamily),
      graphicsFamily(graphicsFamily),
      presentFamily(presentFamily)
  {
  }

  QueueFamilyIndices::QueueFamilyIndices() = default;

  QueueFamilyIndices::QueueFamilyIndices(const QueueFamilyIndices& object): QueueFamilyIndices(
      object.transferFamily,
      object.graphicsFamily,
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
    std::swap(first.transferFamily, second.transferFamily);
    std::swap(first.graphicsFamily, second.graphicsFamily);
    std::swap(first.presentFamily, second.presentFamily);
  }

  bool QueueFamilyIndices::isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
  }

  QueueFamilyIndices::~QueueFamilyIndices() = default;

}
