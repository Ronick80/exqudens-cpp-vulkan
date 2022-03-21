#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct DescriptorSetLayout {

    VkDevice device;
    VkDescriptorSetLayout value;

  };

}
