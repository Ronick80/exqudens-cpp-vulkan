#pragma once

#include <stdexcept>

#include <gtest/gtest.h>

#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/Factory.hpp"

namespace exqudens::vulkan {

  class ShaderTests : public testing::Test, protected Factory {
  };

  TEST_F(ShaderTests, test1) {
    try {
      std::map<std::string, std::string> environmentVariables = createEnvironmentVariables(TestUtils::getExecutableDir());

      for (auto const& [name, value] : environmentVariables) {
        setEnvironmentVariable(name, value);
      }

      Configuration configuration = createConfiguration();
      configuration.presentQueueFamilyRequired = false;
      configuration.deviceExtensions = {};
      std::ostringstream stream;
      Logger logger = createLogger(stream);

      functions = createFunctions();

      VkInstance instance = createInstance(configuration, logger);
      VkDebugUtilsMessengerEXT debugUtilsMessenger = createDebugUtilsMessenger(instance, logger);
      PhysicalDevice physicalDevice = createPhysicalDevice(instance, configuration);
      VkDevice device = createDevice(physicalDevice.value, configuration, physicalDevice.queueFamilyIndexInfo);

      Shader vertexShader1 = createShader(device, "resources/shader/shader-1.vert.spv");
      Shader fragmentShader1 = createShader(device, "resources/shader/shader-1.frag.spv");

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

      destroyShader(vertexShader1, device);
      destroyShader(fragmentShader1, device);

      ASSERT_TRUE(vertexShader2.shaderModule != nullptr);
      ASSERT_TRUE(fragmentShader2.shaderModule != nullptr);

      destroyDevice(device);
      destroyPhysicalDevice(physicalDevice);
      destroyDebugUtilsMessenger(debugUtilsMessenger, instance);
      destroyInstance(instance);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
