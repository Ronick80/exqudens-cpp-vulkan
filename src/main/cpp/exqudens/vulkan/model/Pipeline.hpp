#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Pipeline {

    VkDevice device;
    VkPipelineLayout layout;
    VkPipeline value;

  };

}
