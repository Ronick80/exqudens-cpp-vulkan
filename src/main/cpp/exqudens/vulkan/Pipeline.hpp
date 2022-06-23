#pragma once

#include <optional>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/GraphicsPipelineCreateInfo.hpp"

namespace exqudens::vulkan {

  struct Pipeline {

    class Builder;

    static Builder builder();

    std::vector<vk::DescriptorSetLayout> setLayouts;
    std::vector<vk::PushConstantRange> pushConstantRanges;
    vk::PipelineLayoutCreateInfo layoutCreateInfo;
    std::shared_ptr<vk::raii::PipelineLayout> layout;

    vk::PipelineCacheCreateInfo cacheCreateInfo;
    std::shared_ptr<vk::raii::PipelineCache> cache;

    std::optional<vk::ComputePipelineCreateInfo> computeCreateInfo;
    std::optional<GraphicsPipelineCreateInfo> graphicsCreateInfo;
    std::optional<vk::RayTracingPipelineCreateInfoNV> rayTracingCreateInfo;
    std::map<std::string, std::pair<vk::ShaderModuleCreateInfo, std::shared_ptr<vk::raii::ShaderModule>>> shaders;
    std::shared_ptr<vk::raii::Pipeline> value;

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

    vk::raii::Pipeline& reference() {
      try {
        if (!value) {
          throw std::runtime_error(CALL_INFO() + ": value is not initialized!");
        }
        return *value;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

  };

  class Pipeline::Builder {

    private:

      std::weak_ptr<vk::raii::Device> device;
      std::function<std::vector<char>(const std::string&)> readFileFunction;
      std::vector<vk::DescriptorSetLayout> setLayouts;
      std::vector<vk::PushConstantRange> pushConstantRanges;
      std::optional<vk::PipelineLayoutCreateInfo> layoutCreateInfo;
      std::optional<vk::PipelineCacheCreateInfo> cacheCreateInfo;
      std::optional<vk::ComputePipelineCreateInfo> computeCreateInfo;
      std::optional<GraphicsPipelineCreateInfo> graphicsCreateInfo;
      std::optional<vk::RayTracingPipelineCreateInfoNV> rayTracingCreateInfo;
      std::vector<std::string> paths;

    public:

      Pipeline::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      Pipeline::Builder& setReadFileFunction(const std::function<std::vector<char>(const std::string&)>& val) {
        readFileFunction = val;
        return *this;
      }

      Pipeline::Builder& addSetLayout(const vk::DescriptorSetLayout& val) {
        setLayouts.emplace_back(val);
        return *this;
      }

      Pipeline::Builder& setSetLayouts(const std::vector<vk::DescriptorSetLayout>& val) {
        setLayouts = val;
        return *this;
      }

      Pipeline::Builder& addPushConstantRange(const vk::PushConstantRange& val) {
        pushConstantRanges.emplace_back(val);
        return *this;
      }

      Pipeline::Builder& setPushConstantRanges(const std::vector<vk::PushConstantRange>& val) {
        pushConstantRanges = val;
        return *this;
      }

      Pipeline::Builder& setLayoutCreateInfo(const vk::PipelineLayoutCreateInfo& val) {
        layoutCreateInfo = val;
        return *this;
      }

      Pipeline::Builder& setCacheCreateInfo(const vk::PipelineCacheCreateInfo& val) {
        cacheCreateInfo = val;
        return *this;
      }

      Pipeline::Builder& setComputeCreateInfo(const vk::ComputePipelineCreateInfo& val) {
        computeCreateInfo = val;
        return *this;
      }

      Pipeline::Builder& setGraphicsCreateInfo(const GraphicsPipelineCreateInfo& val) {
        graphicsCreateInfo = val;
        return *this;
      }

      Pipeline::Builder& setRayTracingCreateInfo(const vk::RayTracingPipelineCreateInfoNV& val) {
        rayTracingCreateInfo = val;
        return *this;
      }

      Pipeline::Builder& setPaths(const std::vector<std::string>& val) {
        paths = val;
        return *this;
      }

      Pipeline build() {
        try {
          if (!readFileFunction) {
            readFileFunction = &Utility::readFile;
          }

          Pipeline target = {};
          target.setLayouts = setLayouts;
          target.pushConstantRanges = pushConstantRanges;
          target.layoutCreateInfo = layoutCreateInfo.value_or(vk::PipelineLayoutCreateInfo());
          target.layoutCreateInfo.setSetLayouts(target.setLayouts);
          target.layoutCreateInfo.setPushConstantRanges(target.pushConstantRanges);
          target.layout = std::make_shared<vk::raii::PipelineLayout>(
              *device.lock(),
              target.layoutCreateInfo
          );
          target.cacheCreateInfo = cacheCreateInfo.value_or(vk::PipelineCacheCreateInfo());
          target.cache = std::make_shared<vk::raii::PipelineCache>(
              *device.lock(),
              target.cacheCreateInfo
          );
          target.computeCreateInfo = computeCreateInfo;
          target.graphicsCreateInfo = graphicsCreateInfo;
          target.rayTracingCreateInfo = rayTracingCreateInfo;
          if (graphicsCreateInfo) {
            std::vector<vk::PipelineShaderStageCreateInfo> stages;
            for (const std::string& path : paths) {
              if (!target.shaders.contains(path)) {
                std::vector<char> bytes = readFileFunction(path);
                if (bytes.empty()) {
                  throw std::runtime_error(CALL_INFO() + ": '" + path + "' failed to create shader module bytes is empty!");
                }
                vk::ShaderModuleCreateInfo shaderCreateInfo = vk::ShaderModuleCreateInfo()
                    .setCodeSize(bytes.size())
                    .setPCode(reinterpret_cast<const uint32_t*>(bytes.data()));
                target.shaders[path] = std::make_pair(
                    shaderCreateInfo,
                    std::make_shared<vk::raii::ShaderModule>(*device.lock(), shaderCreateInfo)
                );
                vk::PipelineShaderStageCreateInfo stage = vk::PipelineShaderStageCreateInfo();
                stage.setPName("main");
                stage.setModule(*(*target.shaders[path].second));
                if (path.ends_with(".vert.spv")) {
                  stage.setStage(vk::ShaderStageFlagBits::eVertex);
                } else if (path.ends_with(".frag.spv")) {
                  stage.setStage(vk::ShaderStageFlagBits::eFragment);
                } else {
                  throw std::invalid_argument(CALL_INFO() + ": '" + path + "' failed to create shader!");
                }
                stages.emplace_back(stage);
              }
            }
            target.graphicsCreateInfo.value().setStages(stages);
            target.graphicsCreateInfo.value().setLayout(*target.layoutReference());
            target.value = std::make_shared<vk::raii::Pipeline>(
                *device.lock(),
                target.cacheReference(),
                target.graphicsCreateInfo.value()
            );
          }
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  Pipeline::Builder Pipeline::builder() {
    return {};
  }

}
