#include "exqudens/vulkan/model/Buffer.hpp"

#include <utility>

namespace exqudens::vulkan {

  Buffer::Buffer(
      VkBuffer value,
      VkDeviceMemory memory
  ):
      value(value),
      memory(memory)
  {
  }

  Buffer::Buffer() = default;

  Buffer::Buffer(const Buffer& object): Buffer(
      object.value,
      object.memory
  ) {
  }

  Buffer::Buffer(Buffer&& object) noexcept: Buffer() {
    swap(*this, object);
  }

  Buffer& Buffer::operator=(const Buffer& object) {
    return *this = Buffer(object);
  }

  Buffer& Buffer::operator=(Buffer&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(Buffer& first, Buffer& second) {
    std::swap(first.value, second.value);
    std::swap(first.memory, second.memory);
  }

  Buffer::~Buffer() = default;

}
