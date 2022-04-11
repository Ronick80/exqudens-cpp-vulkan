#pragma once

#include <vector>
#include <iostream>
#include <format>

#include <gtest/gtest.h>

#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/raii/Utility.hpp"

namespace exqudens::vulkan {

  class OtherTests : public testing::Test {

    protected:

      void SetUp() override {
        try {
          std::cout << std::format("{}", CALL_INFO()) << std::endl;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void TearDown() override {
        try {
          std::cout << std::format("{}", CALL_INFO()) << std::endl;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  TEST_F(OtherTests, test1) {
    try {
      std::cout << std::format("{}", "--- 111 ---") << std::endl;

      std::vector<const char*> enabledLayerNames = {};
      std::optional<vk::raii::Context> context = {};

      TestUtils::setEnvironmentVariable("VK_LAYER_PATH", TestUtils::getExecutableDir());

      enabledLayerNames = {"VK_LAYER_KHRONOS_validation"};
      context = vk::raii::Context();

      bool validationSupported = raii::Utility::isValidationLayersSupported(*context, enabledLayerNames);

      if (validationSupported) {
        std::cout << std::format("ON: {}", validationSupported) << std::endl;
      } else {
        std::cout << std::format("OFF: {}", validationSupported) << std::endl;
      }

      std::cout << std::format("{}", "--- 222 ---") << std::endl;
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
