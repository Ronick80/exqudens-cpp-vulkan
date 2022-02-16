#include "exqudens/vulkan/model/Queue.hpp"

#include <utility>

namespace exqudens::vulkan {

  Queue::Queue(
      uint32_t familyIndex,
      VkQueue value
  ):
      familyIndex(familyIndex),
      value(value)
  {
  }

  Queue::Queue() = default;

  Queue::Queue(const Queue& object): Queue(
      object.familyIndex,
      object.value
  ) {
  }

  Queue::Queue(Queue&& object) noexcept: Queue() {
    swap(*this, object);
  }

  Queue& Queue::operator=(const Queue& object) {
    return *this = Queue(object);
  }

  Queue& Queue::operator=(Queue&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(Queue& first, Queue& second) {
    std::swap(first.familyIndex, second.familyIndex);
    std::swap(first.value, second.value);
  }

  Queue::~Queue() = default;

}
