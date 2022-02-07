#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"

#include <utility>

namespace exqudens::vulkan {

  SwapChainSupportDetails::SwapChainSupportDetails(
      VkSurfaceCapabilitiesKHR capabilities,
      std::vector<VkSurfaceFormatKHR> formats,
      std::vector<VkPresentModeKHR> presentModes
  ):
      capabilities(capabilities),
      formats(std::move(formats)),
      presentModes(std::move(presentModes))
  {
  }

  SwapChainSupportDetails::SwapChainSupportDetails() = default;

  SwapChainSupportDetails::SwapChainSupportDetails(const SwapChainSupportDetails& object): SwapChainSupportDetails(
      object.capabilities,
      object.formats,
      object.presentModes
  ) {
  }

  SwapChainSupportDetails::SwapChainSupportDetails(SwapChainSupportDetails&& object) noexcept: SwapChainSupportDetails() {
    swap(*this, object);
  }

  SwapChainSupportDetails& SwapChainSupportDetails::operator=(const SwapChainSupportDetails& object) {
    return *this = SwapChainSupportDetails(object);
  }

  SwapChainSupportDetails& SwapChainSupportDetails::operator=(SwapChainSupportDetails&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(SwapChainSupportDetails& first, SwapChainSupportDetails& second) {
    std::swap(first.capabilities, second.capabilities);
    std::swap(first.formats, second.formats);
    std::swap(first.presentModes, second.presentModes);
  }

  SwapChainSupportDetails::~SwapChainSupportDetails() = default;

}
