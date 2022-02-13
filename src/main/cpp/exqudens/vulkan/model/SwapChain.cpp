#include "exqudens/vulkan/model/SwapChain.hpp"

#include <utility>

namespace exqudens::vulkan {

  SwapChain::SwapChain(
      VkFormat format,
      VkExtent2D extent,
      VkSwapchainKHR value
  ):
      format(format),
      extent(extent),
      value(value)
  {
  }

  SwapChain::SwapChain() = default;

  SwapChain::SwapChain(const SwapChain& object): SwapChain(
      object.format,
      object.extent,
      object.value
  ) {
  }

  SwapChain::SwapChain(SwapChain&& object) noexcept: SwapChain() {
    swap(*this, object);
  }

  SwapChain& SwapChain::operator=(const SwapChain& object) {
    return *this = SwapChain(object);
  }

  SwapChain& SwapChain::operator=(SwapChain&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(SwapChain& first, SwapChain& second) {
    std::swap(first.format, second.format);
    std::swap(first.extent, second.extent);
    std::swap(first.value, second.value);
  }

  SwapChain::~SwapChain() = default;

}
