#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct Image {

    unsigned int id;
    std::shared_ptr<vk::raii::Image> value;

  };

}
