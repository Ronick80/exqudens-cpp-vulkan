#pragma once

#include <string>
#include <sstream>
#include <filesystem>
#include <stdexcept>

#include <gtest/gtest.h>

#include "exqudens/TestMacros.hpp"
#include "exqudens/TestUtils.hpp"

namespace exqudens::vulkan {

  void func3() {
    try {
      throw std::invalid_argument(CALL_INFO() + ": Test error message!");
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void func2() {
    try {
      func3();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  void func1() {
    try {
      func2();
    } catch (...) {
      std::throw_with_nested(std::runtime_error(CALL_INFO()));
    }
  }

  TEST(TestUtilsTests, test1) {
    try {
      std::string thisFilePath = std::filesystem::path(__FILE__).make_preferred().string();
      std::ostringstream out;
      out << "exqudens::vulkan::func3(" + thisFilePath + ":17): Test error message!" << std::endl;
      out << "exqudens::vulkan::func3(" + thisFilePath + ":19)" << std::endl;
      out << "exqudens::vulkan::func2(" + thisFilePath + ":27)" << std::endl;
      out << "exqudens::vulkan::func1(" + thisFilePath + ":35)";
      std::string expected = out.str();
      try {
        func1();
        FAIL() << "(const std::exception& e) not thrown!";
      } catch (const std::exception& e) {
        std::string result = TestUtils::toString(e);
        ASSERT_EQ(expected, result);
      }
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
