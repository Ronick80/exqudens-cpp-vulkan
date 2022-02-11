#include "exqudens/vulkan/model/Vertex.hpp"

#include <utility>
#include <cstddef>

namespace exqudens::vulkan {

  VkVertexInputBindingDescription Vertex::getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};

    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  std::array<VkVertexInputAttributeDescription, 3> Vertex::getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
  }

  Vertex::Vertex(
      glm::vec2 pos,
      glm::vec3 color,
      glm::vec2 texCoord
  ):
      pos(pos),
      color(color),
      texCoord(texCoord)
  {
  }

  Vertex::Vertex() = default;

  Vertex::Vertex(const Vertex& object): Vertex(
      object.pos,
      object.color,
      object.texCoord
  ) {
  }

  Vertex::Vertex(Vertex&& object) noexcept: Vertex() {
    swap(*this, object);
  }

  Vertex& Vertex::operator=(const Vertex& object) {
    return *this = Vertex(object);
  }

  Vertex& Vertex::operator=(Vertex&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(Vertex& first, Vertex& second) {
    std::swap(first.pos, second.pos);
    std::swap(first.color, second.color);
    std::swap(first.texCoord, second.texCoord);
  }

  Vertex::~Vertex() = default;

}
