#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "exqudens/vulkan/model/PipelineViewportStateCreateInfo.hpp"
#include "exqudens/vulkan/model/PipelineColorBlendStateCreateInfo.hpp"

namespace exqudens::vulkan {

  struct GraphicsPipelineCreateInfo {

    VkPipelineCreateFlags                                   flags;
    //std::vector<VkPipelineShaderStageCreateInfo>            stages;
    //std::optional<VkPipelineVertexInputStateCreateInfo>     vertexInputState;
    std::optional<VkPipelineInputAssemblyStateCreateInfo>   inputAssemblyState;
    std::optional<VkPipelineTessellationStateCreateInfo>    tessellationState;
    std::optional<PipelineViewportStateCreateInfo>          viewportState;
    std::optional<VkPipelineRasterizationStateCreateInfo>   rasterizationState;
    std::optional<VkPipelineMultisampleStateCreateInfo>     multisampleState;
    std::optional<VkPipelineDepthStencilStateCreateInfo>    depthStencilState;
    std::optional<PipelineColorBlendStateCreateInfo>        colorBlendState;
    std::optional<VkPipelineDynamicStateCreateInfo>         dynamicState;
    VkPipelineLayout                                        layout;
    VkRenderPass                                            renderPass;
    uint32_t                                                subpass;
    VkPipeline                                              basePipelineHandle;
    int32_t                                                 basePipelineIndex;

  };

}
