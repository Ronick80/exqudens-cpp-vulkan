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
#include "exqudens/vulkan/Functions.hpp"

namespace exqudens::vulkan {

  TEST(Tests, test1) {
    try {
      Functions functions;
      Configuration configuration = functions.createConfiguration();
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
      Functions functions;

      std::map<std::string, std::string> environmentVariables = functions.createEnvironmentVariables(TestConfiguration::getExecutableDir());

      for (auto const& [name, value] : environmentVariables) {
        functions.setEnvironmentVariable(name, value);
      }

      Configuration configuration = functions.createConfiguration();
      configuration.presentQueueFamilyRequired = false;
      std::ostringstream stream;
      Logger logger = functions.createLogger(stream);

      VkInstance instance = functions.createInstance(configuration, logger);
      VkDebugUtilsMessengerEXT debugUtilsMessenger = functions.createDebugUtilsMessenger(instance, logger);
      VkSurfaceKHR surface = functions.createSurface(instance);
      VkPhysicalDevice physicalDevice = functions.createPhysicalDevice(instance, configuration, surface);
      VkDevice device = functions.createDevice(physicalDevice, configuration, surface);

      //std::this_thread::sleep_for(std::chrono::seconds(5));

      ASSERT_TRUE(instance != nullptr);
      ASSERT_TRUE(debugUtilsMessenger != nullptr);
      ASSERT_TRUE(surface == nullptr);
      ASSERT_TRUE(physicalDevice != nullptr);
      ASSERT_TRUE(device != nullptr);

      functions.destroyDevice(device);
      functions.destroyPhysicalDevice(physicalDevice);
      functions.destroyDebugUtilsMessenger(debugUtilsMessenger, instance);
      functions.destroyInstance(instance);
      std::cout << stream.str();
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
