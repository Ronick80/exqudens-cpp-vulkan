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

    public:

      unsigned int id = 0;

      MyClassA() {
        std::cout << std::format("{}", CALL_INFO()) << std::endl;
        id = currentId;
        currentId++;
      }

      ~MyClassA() {
        std::cout << std::format("{}: id: '{}'", CALL_INFO(), id) << std::endl;
      }

  };

  class MyClassB {

    private:

      inline static unsigned int currentId = 1;

    public:

      unsigned int id = 0;
      std::shared_ptr<MyClassA> value = {};

      MyClassB() {
        std::cout << std::format("{}", CALL_INFO()) << std::endl;
        id = currentId;
        currentId++;
      }

      ~MyClassB() {
        std::cout << std::format("{}: id: '{}'", CALL_INFO(), id) << std::endl;
      }

  };

  class MyEnvironment {

    private:

      unsigned int instanceId = 1;

      std::map<unsigned int, std::shared_ptr<MyClassB>> instanceMap = {};

    public:

      MyClassB& createInstance(const unsigned int& id = 0) {
        auto* instance = new MyClassB();
        if (id != 0 && instanceMap.contains(id)) {
          instanceMap.erase(id);
          instance->id = id;
        } else {
          instance->id = instanceId++;
        }
        instance->value = std::make_shared<MyClassA>();
        instanceMap[instance->id] = std::shared_ptr<MyClassB>(instance);
        return *instanceMap[instance->id];
      }

  };

  class OtherTests : public testing::Test {

    protected:

      std::string previousPath = {};

      void SetUp() override {
        try {
          std::cout << std::format("{}", CALL_INFO()) << std::endl;
          previousPath = TestUtils::getEnvironmentVariable("PATH").value_or("");
          TestUtils::setEnvironmentVariable("PATH", TestUtils::getExecutableDir());
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void TearDown() override {
        try {
          std::cout << std::format("{}", CALL_INFO()) << std::endl;
          TestUtils::setEnvironmentVariable("PATH", previousPath);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  TEST_F(OtherTests, test1) {
    try {
      //GTEST_SKIP() << "Skipping test: '" << __FUNCTION__ << "'";
      MyEnvironment environment;

      MyClassB instance1 = environment.createInstance();
      unsigned int instance1Id = instance1.id;
      std::cout << std::format("instance1Id: '{}'", instance1Id) << std::endl;
      std::cout << std::format("instance1.value.id: '{}'", instance1.value->id) << std::endl;

      instance1 = {};
      std::cout << std::format("instance1 cleared") << std::endl;

      instance1 = environment.createInstance(instance1Id);
      instance1Id = instance1.id;
      std::cout << std::format("instance1.id: '{}'", instance1.id) << std::endl;
      std::cout << std::format("instance1.value.id: '{}'", instance1.value->id) << std::endl;
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
