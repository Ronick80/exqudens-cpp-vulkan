#include "exqudens/vulkan/model/PhysicalDevice.hpp"

#include <utility>

namespace exqudens::vulkan {

  PhysicalDevice::PhysicalDevice(
      QueueFamilyIndices queueFamilyIndicesInfo,
      std::optional<SwapChainSupportDetails> swapChainSupportDetails,
      std::set<uint32_t> queueFamilyIndices,
      VkPhysicalDevice value
  ):
      queueFamilyIndicesInfo(std::move(queueFamilyIndicesInfo)),
      swapChainSupportDetails(std::move(swapChainSupportDetails)),
      queueFamilyIndices(std::move(queueFamilyIndices)),
      value(value)
  {
  }

  PhysicalDevice::PhysicalDevice() = default;

  PhysicalDevice::PhysicalDevice(const PhysicalDevice& object): PhysicalDevice(
      object.queueFamilyIndicesInfo,
      object.swapChainSupportDetails,
      object.queueFamilyIndices,
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
    std::swap(first.queueFamilyIndicesInfo, second.queueFamilyIndicesInfo);
    std::swap(first.swapChainSupportDetails, second.swapChainSupportDetails);
    std::swap(first.queueFamilyIndices, second.queueFamilyIndices);
    std::swap(first.value, second.value);
  }

  PhysicalDevice::~PhysicalDevice() = default;

}
