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
#include "exqudens/vulkan/utility/Functions.hpp"

namespace exqudens::vulkan {

  TEST(CoreTests, test1) {
    try {
      Functions functions;
      Configuration configuration = functions.createConfiguration();

      ASSERT_EQ(true, configuration.validationLayersEnabled);
      ASSERT_EQ(std::string("VK_LAYER_KHRONOS_validation"), std::string(configuration.validationLayers[0]));
      ASSERT_EQ(std::string(VK_EXT_DEBUG_UTILS_EXTENSION_NAME), std::string(configuration.extensions[0]));
      ASSERT_EQ(std::string(VK_KHR_SWAPCHAIN_EXTENSION_NAME), std::string(configuration.deviceExtensions[0]));

    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

  TEST(CoreTests, test2) {
    try {
      Functions functions;

      std::map<std::string, std::string> environmentVariables = functions.createEnvironmentVariables(TestConfiguration::getExecutableDir());

      for (auto const& [name, value] : environmentVariables) {
        functions.setEnvironmentVariable(name, value);
      }

      Configuration configuration = functions.createConfiguration();
      configuration.presentQueueFamilyRequired = false;
      configuration.deviceExtensions = {};
      std::ostringstream stream;
      Logger logger = functions.createLogger(stream);

      VkInstance instance = functions.createInstance(configuration, logger);
      VkDebugUtilsMessengerEXT debugUtilsMessenger = functions.createDebugUtilsMessenger(instance, logger);
      VkSurfaceKHR surface = nullptr;//functions.createSurface(instance);
      VkPhysicalDevice physicalDevice = functions.createPhysicalDevice(instance, configuration, surface);
      VkDevice device = functions.createDevice(physicalDevice, configuration, surface);
      VkQueue computeQueue = functions.createComputeQueue(physicalDevice, configuration, surface, device, 0);
      VkQueue transferQueue = functions.createTransferQueue(physicalDevice, configuration, surface, device, 0);
      VkQueue graphicsQueue = functions.createGraphicsQueue(physicalDevice, configuration, surface, device, 0);
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
