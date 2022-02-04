#pragma once

#include <format>
#include <iostream>

#include <gtest/gtest.h>

#include "exqudens/vulkan/Functions.hpp"

namespace exqudens::vulkan {

  TEST(Tests, test1) {
    Configuration configuration = Functions::createConfiguration({"aaa", "bbb"});
    ASSERT_EQ(std::string("aaa"), configuration.extensions.values[0]);
    ASSERT_EQ(std::string("bbb"), configuration.extensions.values[1]);
    ASSERT_EQ(std::string("aaa"), std::string(configuration.extensions.pointers[0]));
    ASSERT_EQ(std::string("bbb"), std::string(configuration.extensions.pointers[1]));
  }

}
