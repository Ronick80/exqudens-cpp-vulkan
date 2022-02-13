#include "exqudens/vulkan/model/SwapChain.hpp"

#include <utility>

namespace exqudens::vulkan {

  SwapChain::SwapChain(
      VkFormat format,
      VkExtent2D extent,
      int width,
      int height,
      VkSwapchainKHR value
  ):
      format(format),
      extent(extent),
      width(width),
      height(height),
      value(value)
  {
  }

  SwapChain::SwapChain() = default;

  SwapChain::SwapChain(const SwapChain& object): SwapChain(
      object.format,
      object.extent,
      object.width,
      object.height,
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
    std::swap(first.width, second.width);
    std::swap(first.height, second.height);
    std::swap(first.value, second.value);
  }

  SwapChain::~SwapChain() = default;

}
