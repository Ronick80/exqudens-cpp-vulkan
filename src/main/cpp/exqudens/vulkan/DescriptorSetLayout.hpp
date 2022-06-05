#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/DescriptorSetLayoutCreateInfo.hpp"
#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct DescriptorSetLayout: Resource<vk::raii::DescriptorSetLayout> {

    //vk::DescriptorSetLayoutCreateFlags flags;
    //std::vector<vk::DescriptorSetLayoutBinding> bindings;
    //vk::DescriptorSetLayoutCreateInfo createInfo;
    DescriptorSetLayoutCreateInfo createInfo;

  };

}
