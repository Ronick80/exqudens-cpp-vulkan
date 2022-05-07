#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/ContextCreateInfo.hpp"

namespace exqudens::vulkan {

  struct Context {

    unsigned int id;
    ContextCreateInfo createInfo;
    std::shared_ptr<vk::raii::Context> value;

  };

}
