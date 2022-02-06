#pragma once

#include <exception>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <format>
#include <iostream>

#include <gtest/gtest.h>

#include "exqudens/TestConfiguration.hpp"
#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/Functions.hpp"

namespace exqudens::vulkan {

  TEST(Tests, test1) {
    try {
      Configuration configuration = Functions::createConfiguration();
      ASSERT_EQ(true, configuration.validationLayersEnabled);

      ASSERT_EQ(std::string("VK_LAYER_KHRONOS_validation"), configuration.validationLayers.values[0]);
      ASSERT_EQ(std::string("VK_LAYER_KHRONOS_validation"), std::string(configuration.validationLayers.pointers[0]));

      ASSERT_EQ(std::string(VK_EXT_DEBUG_UTILS_EXTENSION_NAME), configuration.extensions.values[0]);
      ASSERT_EQ(std::string(VK_EXT_DEBUG_UTILS_EXTENSION_NAME), std::string(configuration.extensions.pointers[0]));

      ASSERT_EQ(std::string(VK_KHR_SWAPCHAIN_EXTENSION_NAME), configuration.deviceExtensions.values[0]);
      ASSERT_EQ(std::string(VK_KHR_SWAPCHAIN_EXTENSION_NAME), std::string(configuration.deviceExtensions.pointers[0]));
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

  TEST(Tests, test2) {
    try {
      std::cout << std::format("executableDir: '{}'", TestConfiguration::getExecutableDir()) << std::endl;
      std::cout << std::format("executableFile: '{}'", TestConfiguration::getExecutableFile()) << std::endl;
      std::vector<std::vector<std::vector<unsigned char>>> image1 = TestUtils::readPng(
          std::filesystem::path(TestConfiguration::getExecutableDir())
              .append("resources")
              .append("png")
              .append("texture.png")
              .make_preferred()
              .string()
      );
      ASSERT_EQ(480, image1.size());
      ASSERT_EQ(640, image1[0].size());
      ASSERT_EQ(4, image1[0][0].size());
      std::cout << "size OK!" << std::endl;
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
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

  TEST(Tests, test3) {
    try {
      std::ostringstream stream;
      Configuration configuration = Functions::createConfiguration(false);
      Logger logger = Functions::createLogger();
      VkInstance instance = Functions::createInstance(configuration, logger);
      std::this_thread::sleep_for(std::chrono::seconds(5));
      vkDestroyInstance(instance, nullptr);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
