#pragma once

#include <format>
#include <iostream>

#include <gtest/gtest.h>

#include "exqudens/test/TestConfiguration.hpp"
#include "exqudens/test/TestUtils.hpp"
#include "exqudens/vulkan/Functions.hpp"

namespace exqudens::vulkan {

  TEST(Tests, test1) {
    Configuration configuration = Functions::createConfiguration();
    ASSERT_EQ(std::string(VK_EXT_DEBUG_UTILS_EXTENSION_NAME), configuration.extensions.values[0]);
    ASSERT_EQ(std::string(VK_EXT_DEBUG_UTILS_EXTENSION_NAME), std::string(configuration.extensions.pointers[0]));
  }

  TEST(Tests, test2) {
    std::cout << std::format("executableDir: '{}'", TestConfiguration::getExecutableDir()) << std::endl;
    std::cout << std::format("executableFile: '{}'", TestConfiguration::getExecutableFile()) << std::endl;
    std::vector<std::vector<unsigned char>> image1 = TestUtils::readPng(
        std::filesystem::path(TestConfiguration::getExecutableDir())
            .append("resources")
            .append("png")
            .append("texture.png")
            .make_preferred()
            .string()
    );
    ASSERT_EQ(640, image1.size());
    ASSERT_EQ(480, image1[0].size());
    TestUtils::writePng(
        image1,
        std::filesystem::path(TestConfiguration::getExecutableDir())
            .append("resources")
            .append("png")
            .append("texture1.png")
            .make_preferred()
            .string()
    );
    ASSERT_EQ(1, 1);
  }

}
