#pragma once

#include <stdexcept>
#include <iostream>
#include <format>

#include <gtest/gtest.h>
#include <vulkan/vulkan.h>

#include "exqudens/TestUtils.hpp"

namespace exqudens::vulkan {

  class OtherTests : public testing::Test {
  };

  TEST_F(OtherTests, test1) {
    try {
      VkMemoryPropertyFlags flags1 = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
      VkMemoryPropertyFlags flags2 = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
      VkMemoryPropertyFlags flags3 = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

      std::cout << std::format("flags1: '{}'", flags1) << std::endl;
      std::cout << std::format("flags2: '{}'", flags2) << std::endl;

      if (
          (flags2 & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
          || (flags2 & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
      ) {
        std::cout << std::format("AAA", flags2) << std::endl;
      }
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
