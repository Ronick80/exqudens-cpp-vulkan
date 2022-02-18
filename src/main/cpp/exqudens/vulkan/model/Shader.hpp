#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Shader {

    VkShaderModule shaderModule;
    VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo;

  };

}
