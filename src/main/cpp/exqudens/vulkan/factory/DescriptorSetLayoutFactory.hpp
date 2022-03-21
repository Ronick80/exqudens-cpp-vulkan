#pragma once

#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/model/DescriptorSetLayoutCreateInfo.hpp"
#include "exqudens/vulkan/model/DescriptorSetLayout.hpp"

namespace exqudens::vulkan {

  class DescriptorSetLayoutFactory : virtual public Utility {

    public:

      virtual DescriptorSetLayout createDescriptorSetLayout(VkDevice& device) = 0;
      virtual DescriptorSetLayout createDescriptorSetLayout(
          VkDevice& device,
          const DescriptorSetLayoutCreateInfo& createInfo
      ) = 0;

      virtual void destroyDescriptorSetLayout(DescriptorSetLayout& descriptorSetLayout) = 0;

  };

}
