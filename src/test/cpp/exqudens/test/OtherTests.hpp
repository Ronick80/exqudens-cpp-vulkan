#pragma once

#include <optional>
#include <stdexcept>
#include <iostream>
#include <format>

#include <gtest/gtest.h>
#include <vulkan/vulkan.h>

#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/MyFactoryBase.hpp"

namespace exqudens::vulkan {

  class OtherTests : public testing::Test {
  };

  TEST_F(OtherTests, test1) {
    try {
      ASSERT_EQ(1, 1);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
