#pragma once

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"
#include "exqudens/vulkan/model/ShaderType.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Shader {

    public:

      ShaderType type = ShaderType::NONE;
      VkShaderModule shaderModule = nullptr;
      VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo{};

      Shader(
          ShaderType type,
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
