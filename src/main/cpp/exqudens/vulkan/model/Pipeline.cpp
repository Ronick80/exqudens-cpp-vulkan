#include "exqudens/vulkan/model/Pipeline.hpp"

#include <utility>

namespace exqudens::vulkan {

  Pipeline::Pipeline(
      VkPipelineLayout layout,
      VkPipeline value
  ):
      layout(layout),
      value(value)
  {
  }

  Pipeline::Pipeline() = default;

  Pipeline::Pipeline(const Pipeline& object): Pipeline(
      object.layout,
      object.value
  ) {
  }

  Pipeline::Pipeline(Pipeline&& object) noexcept: Pipeline() {
    swap(*this, object);
  }

  Pipeline& Pipeline::operator=(const Pipeline& object) {
    return *this = Pipeline(object);
  }

  Pipeline& Pipeline::operator=(Pipeline&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(Pipeline& first, Pipeline& second) {
    std::swap(first.layout, second.layout);
    std::swap(first.value, second.value);
  }

  Pipeline::~Pipeline() = default;

}
