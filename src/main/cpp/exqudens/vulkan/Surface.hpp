#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct Surface {

    unsigned int id;
    std::shared_ptr<vk::raii::SurfaceKHR> value;

  };

}
