#include "exqudens/vulkan/model/QueueFamilyIndexInfo.hpp"

#include <utility>

namespace exqudens::vulkan {

  QueueFamilyIndexInfo::QueueFamilyIndexInfo(
      bool computeFamilyRequired,
      std::optional<uint32_t> computeFamily,

      bool transferFamilyRequired,
      std::optional<uint32_t> transferFamily,

      bool graphicsFamilyRequired,
      std::optional<uint32_t> graphicsFamily,

      bool presentFamilyRequired,
      std::optional<uint32_t> presentFamily,

      bool complete,
      std::set<uint32_t> uniqueQueueFamilyIndices
  ):
      computeFamilyRequired(computeFamilyRequired),
      computeFamily(computeFamily),

      transferFamilyRequired(transferFamilyRequired),
      transferFamily(transferFamily),

      graphicsFamilyRequired(graphicsFamilyRequired),
      graphicsFamily(graphicsFamily),

      presentFamilyRequired(presentFamilyRequired),
      presentFamily(presentFamily),

      complete(complete),
      uniqueQueueFamilyIndices(std::move(uniqueQueueFamilyIndices))
  {
  }

  QueueFamilyIndexInfo::QueueFamilyIndexInfo() = default;

  QueueFamilyIndexInfo::QueueFamilyIndexInfo(const QueueFamilyIndexInfo& object): QueueFamilyIndexInfo(
      object.computeFamilyRequired,
      object.computeFamily,

      object.transferFamilyRequired,
      object.transferFamily,

      object.graphicsFamilyRequired,
      object.graphicsFamily,

      object.presentFamilyRequired,
      object.presentFamily,

      object.complete,
      object.uniqueQueueFamilyIndices
  ) {
  }

  QueueFamilyIndexInfo::QueueFamilyIndexInfo(QueueFamilyIndexInfo&& object) noexcept: QueueFamilyIndexInfo() {
    swap(*this, object);
  }

  QueueFamilyIndexInfo& QueueFamilyIndexInfo::operator=(const QueueFamilyIndexInfo& object) {
    return *this = QueueFamilyIndexInfo(object);
  }

  QueueFamilyIndexInfo& QueueFamilyIndexInfo::operator=(QueueFamilyIndexInfo&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(QueueFamilyIndexInfo& first, QueueFamilyIndexInfo& second) {
    std::swap(first.computeFamilyRequired, second.computeFamilyRequired);
    std::swap(first.computeFamily, second.computeFamily);

    std::swap(first.transferFamilyRequired, second.transferFamilyRequired);
    std::swap(first.transferFamily, second.transferFamily);

    std::swap(first.graphicsFamilyRequired, second.graphicsFamilyRequired);
    std::swap(first.graphicsFamily, second.graphicsFamily);

    std::swap(first.presentFamilyRequired, second.presentFamilyRequired);
    std::swap(first.presentFamily, second.presentFamily);

    std::swap(first.complete, second.complete);
    std::swap(first.uniqueQueueFamilyIndices, second.uniqueQueueFamilyIndices);
  }

  QueueFamilyIndexInfo::~QueueFamilyIndexInfo() = default;

}
