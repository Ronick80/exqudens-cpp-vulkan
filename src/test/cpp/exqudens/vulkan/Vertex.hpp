#pragma once

#include <vector>
#include <cstddef>

#include <glm/glm.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct Vertex {

    static vk::VertexInputBindingDescription getBindingDescription() {
      return vk::VertexInputBindingDescription()
          .setBinding(0)
          .setStride(sizeof(Vertex))
          .setInputRate(vk::VertexInputRate::eVertex);
    }

    static std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions() {
      uint32_t posOffset = offsetof(Vertex, pos);
      uint32_t colorOffset = offsetof(Vertex, color);
      uint32_t texCoordOffset = offsetof(Vertex, texCoord);
      return {
          vk::VertexInputAttributeDescription()
              .setBinding(0)
              .setLocation(0)
              .setFormat(vk::Format::eR32G32B32Sfloat)
              .setOffset(posOffset),
          vk::VertexInputAttributeDescription()
              .setBinding(0)
              .setLocation(1)
              .setFormat(vk::Format::eR32G32B32Sfloat)
              .setOffset(colorOffset),
          vk::VertexInputAttributeDescription()
              .setBinding(0)
              .setLocation(2)
              .setFormat(vk::Format::eR32G32Sfloat)
              .setOffset(texCoordOffset)
      };
    }

    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

  };

}
