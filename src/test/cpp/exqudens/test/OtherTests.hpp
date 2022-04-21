#pragma once

#include <string>
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
      std::vector<std::string> names = {"111", "222", "aaa", "bbb", "ccc", "333", "444"};
      int index = 3;

      std::cout << std::format("--- 1 ---") << std::endl;
      for (const std::string& name : names) {
        std::cout << std::format("{}", name) << std::endl;
      }

      names.erase(names.begin(), names.begin() + index);
      names.erase(names.begin() + 1, names.end());

      std::cout << std::format("--- 2 ---") << std::endl;
      for (const std::string& name : names) {
        std::cout << std::format("{}", name) << std::endl;
      }
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
