#pragma once

#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/model/RenderPassCreateInfo.hpp"
#include "exqudens/vulkan/model/RenderPass.hpp"

namespace exqudens::vulkan {

  class RenderPassFactory : virtual public Utility {

    public:

      virtual RenderPass createRenderPass(VkDevice& device, VkFormat& format) = 0;
      virtual RenderPass createRenderPass(VkDevice& device, const RenderPassCreateInfo& createInfo) = 0;

      virtual void destroyRenderPass(RenderPass& renderPass) = 0;

  };

}
