#pragma once

#include <cstdlib>

#include <gtest/gtest.h>

#include "exqudens/TestConfiguration.hpp"
#include "exqudens/TestUiApplication.hpp"

namespace exqudens::vulkan {

  TEST(UiTests, test1) {
    int argc = 0;
    char** argv = nullptr;
    int result = TestUiApplication(argc, argv).run();
    ASSERT_EQ(EXIT_SUCCESS, result);
  }

  TEST(UiTests, test2) {
    ASSERT_EQ(1, 2);
  }

}
