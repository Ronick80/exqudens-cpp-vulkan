#pragma once

#include <string>
#include <vector>
#include <map>
#include <exception>
#include <sstream>
#include <thread>
#include <chrono>
#include <format>
#include <iostream>

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "exqudens/TestConfiguration.hpp"
#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/utility/Factory.hpp"

namespace exqudens::vulkan {

  class CoreTests : public testing::Test {

      //

  };

  TEST_F(CoreTests, test1) {
    try {
      Factory factory;
      Configuration configuration = factory.createConfiguration();

      ASSERT_EQ(true, configuration.validationLayersEnabled);
      ASSERT_EQ(std::string("VK_LAYER_KHRONOS_validation"), std::string(configuration.validationLayers[0]));
      ASSERT_EQ(std::string(VK_EXT_DEBUG_UTILS_EXTENSION_NAME), std::string(configuration.extensions[0]));
      ASSERT_EQ(std::string(VK_KHR_SWAPCHAIN_EXTENSION_NAME), std::string(configuration.deviceExtensions[0]));

    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

  TEST_F(CoreTests, test2) {
    try {
      Factory factory;

      std::map<std::string, std::string> environmentVariables = factory.createEnvironmentVariables(TestConfiguration::getExecutableDir());

      for (auto const& [name, value] : environmentVariables) {
        factory.setEnvironmentVariable(name, value);
      }

      Configuration configuration = factory.createConfiguration();
      configuration.presentQueueFamilyRequired = false;
      configuration.deviceExtensions = {};
      std::ostringstream stream;
      Logger logger = factory.createLogger(stream);

      VkInstance instance = factory.createInstance(configuration, logger);
      VkDebugUtilsMessengerEXT debugUtilsMessenger = factory.createDebugUtilsMessenger(instance, logger);
      VkSurfaceKHR surface = nullptr;//functions.createSurface(instance);
      VkPhysicalDevice physicalDevice = factory.createPhysicalDevice(instance, configuration, surface);
      VkDevice device = factory.createDevice(physicalDevice, configuration, surface);

      Shader vertexShader1 = factory.createShader(device, "resources/shader/shader.vert.spv");
      Shader fragmentShader1 = factory.createShader(device, "resources/shader/shader.frag.spv");

      ASSERT_TRUE(vertexShader1.shaderModule != nullptr);
      ASSERT_EQ(vertexShader1.pipelineShaderStageCreateInfo.stage, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT);
      ASSERT_TRUE(fragmentShader1.shaderModule != nullptr);
      ASSERT_EQ(fragmentShader1.pipelineShaderStageCreateInfo.stage, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT);

      Shader vertexShader2 = vertexShader1;
      Shader fragmentShader2 = fragmentShader1;

      ASSERT_TRUE(vertexShader2.shaderModule != nullptr);
      ASSERT_EQ(vertexShader2.pipelineShaderStageCreateInfo.stage, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT);
      ASSERT_TRUE(fragmentShader2.shaderModule != nullptr);
      ASSERT_EQ(fragmentShader2.pipelineShaderStageCreateInfo.stage, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT);

      factory.destroyShader(vertexShader1, device);
      factory.destroyShader(fragmentShader1, device);

      ASSERT_TRUE(vertexShader2.shaderModule != nullptr);
      ASSERT_TRUE(fragmentShader2.shaderModule != nullptr);

      factory.destroyDevice(device);
      factory.destroyPhysicalDevice(physicalDevice);
      factory.destroyDebugUtilsMessenger(debugUtilsMessenger, instance);
      factory.destroyInstance(instance);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

  TEST_F(CoreTests, test3) {
    try {
      Factory factory;

      std::map<std::string, std::string> environmentVariables = factory.createEnvironmentVariables(TestConfiguration::getExecutableDir());

      for (auto const& [name, value] : environmentVariables) {
        factory.setEnvironmentVariable(name, value);
      }

      Configuration configuration = factory.createConfiguration();
      configuration.presentQueueFamilyRequired = false;
      configuration.deviceExtensions = {};
      std::ostringstream stream;
      Logger logger = factory.createLogger(stream);

      VkInstance instance = factory.createInstance(configuration, logger);
      VkDebugUtilsMessengerEXT debugUtilsMessenger = factory.createDebugUtilsMessenger(instance, logger);
      VkSurfaceKHR surface = nullptr;//functions.createSurface(instance);
      VkPhysicalDevice physicalDevice = factory.createPhysicalDevice(instance, configuration, surface);
      VkDevice device = factory.createDevice(physicalDevice, configuration, surface);
      VkQueue computeQueue = factory.createComputeQueue(physicalDevice, configuration, surface, device, 0);
      VkQueue transferQueue = factory.createTransferQueue(physicalDevice, configuration, surface, device, 0);
      VkQueue graphicsQueue = factory.createGraphicsQueue(physicalDevice, configuration, surface, device, 0);
      VkQueue presentQueue = nullptr;//functions.createPresentQueue(physicalDevice, configuration, surface, device, 0);
      VkSwapchainKHR swapChain = nullptr;//functions.createSwapChain(physicalDevice, configuration, surface, device, 800, 600);

      //std::this_thread::sleep_for(std::chrono::seconds(5));

      ASSERT_TRUE(instance != nullptr);
      ASSERT_TRUE(debugUtilsMessenger != nullptr);
      ASSERT_TRUE(surface == nullptr);
      ASSERT_TRUE(physicalDevice != nullptr);
      ASSERT_TRUE(device != nullptr);
      ASSERT_TRUE(computeQueue != nullptr);
      ASSERT_TRUE(transferQueue != nullptr);
      ASSERT_TRUE(graphicsQueue != nullptr);
      ASSERT_TRUE(presentQueue == nullptr);
      ASSERT_TRUE(swapChain == nullptr);

      factory.destroyDevice(device);
      factory.destroyPhysicalDevice(physicalDevice);
      factory.destroyDebugUtilsMessenger(debugUtilsMessenger, instance);
      factory.destroyInstance(instance);
      std::cout << stream.str();
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
