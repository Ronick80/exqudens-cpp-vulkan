#pragma once

#include <memory>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <format>

#include <gtest/gtest.h>

#include "TestUtils.hpp"

namespace exqudens::vulkan {

  class MyClassA {

    private:

      inline static int currentId = 1;
      int id = -1;

    public:

      MyClassA() {
        std::cout << std::format("{}", CALL_INFO()) << std::endl;
        id = currentId;
        currentId++;
      }

      MyClassA& setId(const int& id) {
        this->id = id;
        return *this;
      }

      int getId() {
        return id;
      }

      ~MyClassA() {
        std::cout << std::format("{}", CALL_INFO()) << std::endl;
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

      MyClassA* create() {
        std::cout << std::format("{}", CALL_INFO()) << std::endl;
        auto* a = new MyClassA;
        return a;
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
      std::shared_ptr<MyClassA> obj1 = {};
      obj1 = std::shared_ptr<MyClassA>(create()); // std::make_shared<MyClassA>(MyClassA());
      std::shared_ptr<MyClassA> obj2 = obj1;

      if (obj1) {
        std::cout << obj1->getId() << std::endl;
      }

      ASSERT_EQ(1, obj1->getId());
      ASSERT_EQ(1, obj2->getId());

      obj1->setId(11);

      ASSERT_EQ(11, obj1->getId());
      ASSERT_EQ(11, obj2->getId());
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
