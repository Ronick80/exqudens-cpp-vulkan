#include "exqudens/vulkan/model/Image.hpp"

#include <utility>

namespace exqudens::vulkan {

  Image::Image(
      VkImage value,
      VkDeviceMemory memory
  ):
      value(value),
      memory(memory)
  {
  }

  Image::Image() = default;

  Image::Image(const Image& object): Image(
      object.value,
      object.memory
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
    std::swap(first.value, second.value);
    std::swap(first.memory, second.memory);
  }

  Image::~Image() = default;

}
