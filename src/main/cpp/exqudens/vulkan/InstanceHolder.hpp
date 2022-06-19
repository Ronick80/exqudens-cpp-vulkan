#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Instance.hpp"

namespace exqudens::vulkan {

  struct InstanceHolder {

    Instance target;
    std::shared_ptr<vk::raii::Context> context;
    std::shared_ptr<vk::raii::Instance> instance;
    std::shared_ptr<Messenger> messenger;
    std::shared_ptr<vk::raii::DebugUtilsMessengerEXT> debugUtilsMessenger;

  };

}
