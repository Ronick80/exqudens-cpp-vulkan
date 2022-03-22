#pragma once

#include <optional>
#include <stdexcept>
#include <iostream>
#include <format>

#include <gtest/gtest.h>
#include <vulkan/vulkan.h>

#include "exqudens/TestUtils.hpp"
#include "exqudens/TestConfiguration.hpp"
#include "exqudens/vulkan/ContextBase.hpp"

namespace exqudens::vulkan {

  class OtherTests : public testing::Test {
  };

  TEST_F(OtherTests, test1) {
    try {
      Context* context = new ContextBase;

      std::map<std::string, std::string> environmentVariables = {};
      Configuration configuration = {};
      Logger logger = {};
      Instance instance = {};
      DebugUtilsMessenger debugUtilsMessenger = {};
      PhysicalDevice physicalDevice = {};
      Device device = {};

      environmentVariables = context->createEnvironmentVariables(TestConfiguration::getExecutableDir());

      for (auto const& [name, value] : environmentVariables) {
        context->setEnvironmentVariable(name, value);
      }

      configuration = context->createConfiguration();
      configuration.presentQueueFamilyRequired = false;
      configuration.deviceExtensions = {};
      logger = context->createLogger(
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
      );

      instance = context->createInstance(configuration, logger);
      debugUtilsMessenger = context->createDebugUtilsMessenger(instance.value, logger);
      physicalDevice = context->createPhysicalDevice(instance.value, configuration);
      device = context->createDevice(physicalDevice.value, configuration, physicalDevice.queueFamilyIndexInfo);

      context->destroy();

      delete context;

      ASSERT_EQ(1, 1);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
