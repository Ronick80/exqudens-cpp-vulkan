#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct Queue {

    unsigned int id;
    uint32_t familyIndex;
    uint32_t index;
    std::shared_ptr<vk::raii::Queue> value;

  };

}
