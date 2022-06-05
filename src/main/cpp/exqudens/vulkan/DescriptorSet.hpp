#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/DescriptorSetAllocateInfo.hpp"
#include "exqudens/vulkan/WriteDescriptorSet.hpp"
#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct DescriptorSet: Resource<vk::raii::DescriptorSet> {

    DescriptorSetAllocateInfo createInfo;
    std::vector<WriteDescriptorSet> writes;

  };

}
