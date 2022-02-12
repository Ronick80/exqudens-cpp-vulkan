#include "exqudens/vulkan/model/Shader.hpp"

#include <utility>

namespace exqudens::vulkan {

  Shader::Shader(
      VkShaderModule shaderModule,
      VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo
  ):
      shaderModule(shaderModule),
      pipelineShaderStageCreateInfo(pipelineShaderStageCreateInfo)
  {
  }

  Shader::Shader() = default;

  Shader::Shader(const Shader& object): Shader(
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
    std::swap(first.shaderModule, second.shaderModule);
    std::swap(first.pipelineShaderStageCreateInfo, second.pipelineShaderStageCreateInfo);
  }

  Shader::~Shader() = default;

}
