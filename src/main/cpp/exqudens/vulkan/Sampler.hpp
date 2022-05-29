#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct Sampler: Resource<vk::raii::Sampler> {

    vk::SamplerCreateInfo createInfo;

  };

}
