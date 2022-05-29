#pragma once

#include <string>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <iostream>
#include <format>

#include <gtest/gtest.h>

#include "TestUtils.hpp"

namespace exqudens::vulkan {

  class MyClassA {

    private:

      inline static unsigned int currentId = 1;
      unsigned int id = 0;

    public:

      MyClassA() {
        std::cout << std::format("{}", CALL_INFO()) << std::endl;
        id = currentId;
        currentId++;
      }

      MyClassA& setId(const unsigned int& id) {
        this->id = id;
        return *this;
      }

      unsigned int getId() {
        return id;
      }

      ~MyClassA() {
        std::cout << std::format("{}", CALL_INFO()) << std::endl;
      }

  };

  class MyEnvironment {

    private:

      unsigned int instanceId = 1;

      std::map<unsigned int, std::shared_ptr<MyClassA>> instanceMap = {};

    public:

      MyClassA& createInstance() {
        auto* instance = new MyClassA();
        std::cout << "AAA" << std::endl;
        instance->setId(instanceId++);
        instanceMap[instance->getId()] = std::shared_ptr<MyClassA>(instance);
        std::cout << "BBB" << std::endl;
        return *instanceMap[instance->getId()];
      }

  };

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
      GTEST_SKIP() << "Skipping test: '" << __FUNCTION__ << "'";
      MyEnvironment environment;

      MyClassA& instance = environment.createInstance();
      std::cout << "CCC" << std::endl;

      ASSERT_EQ(1, instance.getId());
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

  TEST_F(OtherTests, test2) {
    try {
      //GTEST_SKIP() << "Skipping test: '" << __FUNCTION__ << "'";
      unsigned int index = 1;
      std::vector<std::string> names = std::vector<std::string>(5);
      std::generate_n(names.begin(), names.size(), [&index]() { return std::string("aaa: ") + std::to_string(index++); });
      std::ranges::for_each(names, [](auto && o1) { std::cout << o1 << std::endl; });
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
