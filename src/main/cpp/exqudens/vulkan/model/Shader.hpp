#pragma once

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Shader {

    public:

      VkShaderModule shaderModule = nullptr;
      VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};

      Shader(
          VkShaderModule shaderModule,
          VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo
      );
      Shader();
      Shader(const Shader& object);
      Shader(Shader&& object) noexcept;

      Shader& operator=(const Shader& object);
      Shader& operator=(Shader&& object) noexcept;

      friend void swap(Shader& first, Shader& second);

      virtual ~Shader();

  };

}
