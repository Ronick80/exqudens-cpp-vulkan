#pragma once

#include <optional>
#include <stdexcept>
#include <iostream>
#include <format>

#include <gtest/gtest.h>
#include <vulkan/vulkan.h>

#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/Factory.hpp"

namespace exqudens::vulkan {

  class OtherTests : public testing::Test {
  };

  TEST_F(OtherTests, test1) {
    try {
      RenderPassCreateInfo createInfo = {
          .subPasses = {
              SubPassDescription {
                  .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                  .colorAttachments = {
                      VkAttachmentReference {
                          .attachment = 0,
                          .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                      }
                  },
                  .depthStencilAttachment = VkAttachmentReference {
                      .attachment = 1,
                      .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                  }
              }
          }
      };
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
