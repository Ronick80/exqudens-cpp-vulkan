#pragma once

#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/model/Fence.hpp"

namespace exqudens::vulkan {

  class FenceFactory : virtual public Utility {

    public:

      virtual Fence createFence(VkDevice& device) = 0;
      virtual Fence createFence(VkDevice& device, VkFenceCreateFlags flags) = 0;
      virtual std::vector<Fence> createFences(VkDevice& device, std::size_t size) = 0;
      virtual std::vector<Fence> createFences(VkDevice& device, VkFenceCreateFlags flags, std::size_t size) = 0;

      virtual void destroyFence(Fence& fence) = 0;
      virtual void destroyFences(std::vector<Fence>& fences) = 0;

  };

}
