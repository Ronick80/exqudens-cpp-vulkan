#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct DescriptorPool {

    VkDevice device;
    VkDescriptorPool value;

  };

}
