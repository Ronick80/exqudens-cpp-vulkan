#include "exqudens/vulkan/model/PhysicalDevice.hpp"

#include <utility>

namespace exqudens::vulkan {

  PhysicalDevice::PhysicalDevice(
      QueueFamilyIndexInfo queueFamilyIndexInfo,
      std::optional<SwapChainSupportDetails> swapChainSupportDetails,
      VkPhysicalDevice value
  ):
      queueFamilyIndexInfo(std::move(queueFamilyIndexInfo)),
      swapChainSupportDetails(std::move(swapChainSupportDetails)),
      value(value)
  {
  }

  PhysicalDevice::PhysicalDevice() = default;

  PhysicalDevice::PhysicalDevice(const PhysicalDevice& object): PhysicalDevice(
      object.queueFamilyIndexInfo,
      object.swapChainSupportDetails,
      object.value
  ) {
  }

  PhysicalDevice::PhysicalDevice(PhysicalDevice&& object) noexcept: PhysicalDevice() {
    swap(*this, object);
  }

  PhysicalDevice& PhysicalDevice::operator=(const PhysicalDevice& object) {
    return *this = PhysicalDevice(object);
  }

  PhysicalDevice& PhysicalDevice::operator=(PhysicalDevice&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(PhysicalDevice& first, PhysicalDevice& second) {
    std::swap(first.queueFamilyIndexInfo, second.queueFamilyIndexInfo);
    std::swap(first.swapChainSupportDetails, second.swapChainSupportDetails);
    std::swap(first.value, second.value);
  }

  PhysicalDevice::~PhysicalDevice() = default;

}
