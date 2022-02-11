#pragma once

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Pipeline {

    public:

      VkPipelineLayout layout = nullptr;
      VkPipeline value = nullptr;

      Pipeline(
          VkPipelineLayout layout,
          VkPipeline value
      );
      Pipeline();
      Pipeline(const Pipeline& object);
      Pipeline(Pipeline&& object) noexcept;

      Pipeline& operator=(const Pipeline& object);
      Pipeline& operator=(Pipeline&& object) noexcept;

      friend void swap(Pipeline& first, Pipeline& second);

      virtual ~Pipeline();

  };

}
