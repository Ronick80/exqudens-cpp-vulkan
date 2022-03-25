#pragma once

#include <optional>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <format>

#include <gtest/gtest.h>
#include <vulkan/vulkan.h>

#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/ContextBase.hpp"

namespace exqudens::vulkan {

  class OtherTests : public testing::Test {

    protected:

      void SetUp() override {
        try {
          std::cout << std::format("{}", CALL_INFO()) << std::endl;
          std::cout << std::format("PATH: '{}'", TestUtils::getEnvironmentVariable("PATH").value_or("")) << std::endl;
          std::cout << std::format("Path: '{}'", TestUtils::getEnvironmentVariable("Path").value_or("")) << std::endl;
        } catch (const std::exception& e) {
          FAIL() << TestUtils::toString(e);
        }
      }

      void TearDown() override {
        try {
          std::cout << std::format("{}", CALL_INFO()) << std::endl;
        } catch (const std::exception& e) {
          FAIL() << TestUtils::toString(e);
        }
      }

  };

  TEST_F(OtherTests, test1) {
    try {
      std::map<std::string, std::string> environmentVariables = {};
      Configuration configuration = {};
      Logger logger = {};
      Instance instance = {};
      DebugUtilsMessenger debugUtilsMessenger = {};
      PhysicalDevice physicalDevice = {};
      Device device = {};
      Queue graphicsQueue = {};
      CommandPool graphicsCommandPool = {};
      Image image = {};
      ImageView imageView = {};

      Factory* factory = new FactoryBase;
      Context* context = new ContextBase;

      environmentVariables = context->createEnvironmentVariables(TestUtils::getExecutableDir());
      for (auto const& [name, value] : environmentVariables) {
        context->setEnvironmentVariable(name, value);
      }

      configuration = context->createConfiguration();
      configuration.presentQueueFamilyRequired = false;
      configuration.deviceExtensions = {};

      logger = context->createLogger();

      instance = factory->createInstance(configuration, logger);
      debugUtilsMessenger = context->createDebugUtilsMessenger(instance.value, logger);
      physicalDevice = context->createPhysicalDevice(instance.value, configuration);
      device = context->createDevice(physicalDevice.value, configuration, physicalDevice.queueFamilyIndexInfo);
      graphicsQueue = context->createQueue(device.value, physicalDevice.queueFamilyIndexInfo.graphicsFamily.value(), 0);
      graphicsCommandPool = context->createCommandPool(device.value, graphicsQueue.familyIndex);
      image = context->createImage(physicalDevice.value, device.value, 800, 600, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      imageView = context->createImageView(device.value, image.value, image.format);

      context->destroyImageView(imageView);
      context->destroy();

      factory->destroyInstance(instance);

      delete factory;
      delete context;
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
