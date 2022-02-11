#include "exqudens/vulkan/model/Shader.hpp"

#include <utility>

namespace exqudens::vulkan {

  Shader::Shader(
      ShaderType type,
      VkShaderModule shaderModule,
      VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo
  ):
      type(type),
      shaderModule(shaderModule),
      pipelineShaderStageCreateInfo(pipelineShaderStageCreateInfo)
  {
  }

  Shader::Shader() = default;

  Shader::Shader(const Shader& object): Shader(
      object.type,
      object.shaderModule,
      object.pipelineShaderStageCreateInfo
  ) {
  }

  Shader::Shader(Shader&& object) noexcept: Shader() {
    swap(*this, object);
  }

  Shader& Shader::operator=(const Shader& object) {
    return *this = Shader(object);
  }

  Shader& Shader::operator=(Shader&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(Shader& first, Shader& second) {
    std::swap(first.type, second.type);
    std::swap(first.shaderModule, second.shaderModule);
    std::swap(first.pipelineShaderStageCreateInfo, second.pipelineShaderStageCreateInfo);
  }

  Shader::~Shader() = default;

}
