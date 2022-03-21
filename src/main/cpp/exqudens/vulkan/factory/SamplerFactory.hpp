#pragma once

#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/model/Sampler.hpp"

namespace exqudens::vulkan {

  class SamplerFactory : virtual public Utility {

    public:

      virtual Sampler createSampler(VkPhysicalDevice& physicalDevice, VkDevice& device) = 0;

      virtual void destroySampler(Sampler& sampler) = 0;

  };

}
