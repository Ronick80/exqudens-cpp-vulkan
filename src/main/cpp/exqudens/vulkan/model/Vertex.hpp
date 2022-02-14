#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Vertex {

    public:

      static VkVertexInputBindingDescription getBindingDescription();

      static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

      glm::vec2 pos = {};
      glm::vec3 color = {};
      glm::vec2 texCoord = {};

      Vertex(
          glm::vec2 pos,
          glm::vec3 color,
          glm::vec2 texCoord
      );
      Vertex();
      Vertex(const Vertex& object);
      Vertex(Vertex&& object) noexcept;

      Vertex& operator=(const Vertex& object);
      Vertex& operator=(Vertex&& object) noexcept;

      friend void swap(Vertex& first, Vertex& second);

      virtual ~Vertex();

  };

}
