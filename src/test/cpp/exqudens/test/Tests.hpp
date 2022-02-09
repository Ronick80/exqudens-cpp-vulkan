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

#include "exqudens/TestConfiguration.hpp"
#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/function/UtilFunctions.hpp"
#include "exqudens/vulkan/function/CreateFunctions.hpp"
#include "exqudens/vulkan/function/DestroyFunctions.hpp"

namespace exqudens::vulkan {

  TEST(Tests, test1) {
    try {
      CreateFunctions createFunctions;
      Configuration configuration = createFunctions.createConfiguration();
      ASSERT_EQ(true, configuration.validationLayersEnabled);

      ASSERT_EQ(std::string("VK_LAYER_KHRONOS_validation"), configuration.validationLayers.values[0]);
      ASSERT_EQ(std::string("VK_LAYER_KHRONOS_validation"), std::string(configuration.validationLayers.pointers[0]));

      ASSERT_EQ(std::string(VK_EXT_DEBUG_UTILS_EXTENSION_NAME), configuration.extensions.values[0]);
      ASSERT_EQ(std::string(VK_EXT_DEBUG_UTILS_EXTENSION_NAME), std::string(configuration.extensions.pointers[0]));

      ASSERT_EQ(std::string(VK_KHR_SWAPCHAIN_EXTENSION_NAME), configuration.deviceExtensions.values[0]);
      ASSERT_EQ(std::string(VK_KHR_SWAPCHAIN_EXTENSION_NAME), std::string(configuration.deviceExtensions.pointers[0]));
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

  TEST(Tests, test2) {
    try {
      UtilFunctions utilFunctions;
      CreateFunctions createFunctions(&utilFunctions);
      DestroyFunctions destroyFunctions;

      std::map<std::string, std::string> environmentVariables = createFunctions.createEnvironmentVariables(TestConfiguration::getExecutableDir());

      for (auto const& [name, value] : environmentVariables) {
        utilFunctions.setEnvironmentVariable(name, value);
      }

      Configuration configuration = createFunctions.createConfiguration();
      configuration.presentQueueFamilyRequired = false;
      std::ostringstream stream;
      Logger logger = createFunctions.createLogger(stream);

      VkInstance instance = createFunctions.createInstance(configuration, logger);
      VkDebugUtilsMessengerEXT debugUtilsMessenger = createFunctions.createDebugUtilsMessenger(instance, logger);
      VkPhysicalDevice physicalDevice = createFunctions.createPhysicalDevice(instance, configuration);

      //std::this_thread::sleep_for(std::chrono::seconds(5));

      destroyFunctions.destroyPhysicalDevice(physicalDevice);
      destroyFunctions.destroyDebugUtilsMessenger(debugUtilsMessenger, instance);
      destroyFunctions.destroyInstance(instance);
      std::cout << stream.str();
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
