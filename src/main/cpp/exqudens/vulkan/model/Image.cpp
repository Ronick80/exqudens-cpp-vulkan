#include "exqudens/vulkan/model/Image.hpp"

#include <utility>

namespace exqudens::vulkan {

  Image::Image(
      uint32_t width,
      uint32_t height,
      VkFormat format,
      VkDeviceMemory memory,
      VkImage value
  ):
      width(width),
      height(height),
      format(format),
      value(value),
      memory(memory)
  {
  }

  Image::Image() = default;

  Image::Image(const Image& object): Image(
      object.width,
      object.height,
      object.format,
      object.memory,
      object.value
  ) {
  }

  Image::Image(Image&& object) noexcept: Image() {
    swap(*this, object);
  }

  Image& Image::operator=(const Image& object) {
    return *this = Image(object);
  }

  Image& Image::operator=(Image&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(Image& first, Image& second) {
    std::swap(first.width, second.width);
    std::swap(first.height, second.height);
    std::swap(first.format, second.format);
    std::swap(first.memory, second.memory);
    std::swap(first.value, second.value);
  }

  Image::~Image() = default;

}
