#pragma once

#include <optional>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/GraphicsPipelineCreateInfo.hpp"
#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  struct Pipeline: Resource<vk::raii::Pipeline> {

    vk::PipelineCacheCreateInfo cacheCreateInfo;
    std::shared_ptr<vk::raii::PipelineCache> cache;
    vk::PipelineLayoutCreateInfo layoutCreateInfo;
    std::shared_ptr<vk::raii::PipelineLayout> layout;
    std::optional<vk::ComputePipelineCreateInfo> computePipelineCreateInfo;
    std::optional<GraphicsPipelineCreateInfo> graphicsPipelineCreateInfo;
    std::optional<vk::RayTracingPipelineCreateInfoNV> rayTracingPipelineCreateInfoNV;

    vk::raii::PipelineLayout& layoutReference() {
      try {
        if (!layout) {
          throw std::runtime_error(CALL_INFO() + ": layout is not initialized!");
        }
        return *layout;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

    vk::raii::PipelineCache& cacheReference() {
      try {
        if (!cache) {
          throw std::runtime_error(CALL_INFO() + ": cache is not initialized!");
        }
        return *cache;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

  };

}
