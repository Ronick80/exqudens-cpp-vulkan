#pragma once

#include <cstring>
#include <memory>
#include <string>
#include <optional>
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>
#include <format>

#include <gtest/gtest.h>
#include <vulkan/vulkan_raii.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TestUtils.hpp"
#include "exqudens/vulkan/Vertex.hpp"
#include "exqudens/vulkan/UniformBufferObject.hpp"
#include "exqudens/vulkan/Environment.hpp"

namespace exqudens::vulkan {

  class UiTestsA : public testing::Test {

    protected:

      class Renderer {

        public:

          bool resized = false;

        private:

          std::vector<Vertex> vertices = {};
          std::vector<uint16_t> indices = {};

          std::shared_ptr<Environment> environment = {};
          std::shared_ptr<Context> context = {};
          std::shared_ptr<Instance> instance = {};
          std::shared_ptr<Messenger> messenger = {};
          std::shared_ptr<DebugUtilsMessenger> debugUtilsMessenger = {};
          std::shared_ptr<Surface> surface = {};
          std::shared_ptr<PhysicalDevice> physicalDevice = {};
          std::shared_ptr<Device> device = {};
          std::shared_ptr<SwapChain> swapChain = {};

        public:

          void create(const std::vector<std::string>& arguments, GLFWwindow* window, uint32_t width, uint32_t height) {
            try {
              vertices = {
                  {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                  {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                  {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                  {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

                  {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                  {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                  {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                  {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
              };

              indices = {
                  0, 1, 2, 2, 3, 0,
                  4, 5, 6, 6, 7, 4
              };

              environment = std::make_shared<Environment>();

              context = environment->createContext(
                  ContextCreateInfo()
                      .setEnvironmentVariables({{"VK_LAYER_PATH", arguments.front().c_str()}})
                      .setEnabledLayerNames({"VK_LAYER_KHRONOS_validation"})
                      .setEnabledExtensionNames({VK_EXT_DEBUG_UTILS_EXTENSION_NAME})
                      .setEnabledDeviceExtensionNames({VK_KHR_SWAPCHAIN_EXTENSION_NAME})
              );

              uint32_t glfwExtensionCount = 0;
              const char** glfwExtensions;
              glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
              std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
              for (const char*& extension : extensions) {
                context->createInfo.enabledExtensionNames.emplace_back(extension);
              }

              instance = environment->createInstance(
                  *context,
                  vk::ApplicationInfo()
                      .setPApplicationName("Exqudens Application")
                      .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
                      .setPEngineName("Exqudens Engine")
                      .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
                      .setApiVersion(VK_API_VERSION_1_0),
                  vk::InstanceCreateInfo()
                      .setPEnabledExtensionNames(context->createInfo.enabledExtensionNames)
                      .setPEnabledLayerNames(context->createInfo.enabledLayerNames)
              );

              messenger = environment->createMessenger(
                  std::cout,
                  vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                  vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose,
                  {}
              );

              debugUtilsMessenger = environment->createDebugUtilsMessenger(
                  *instance,
                  *messenger,
                  vk::DebugUtilsMessengerCreateInfoEXT()
                      .setPUserData(messenger.get())
                      .setPfnUserCallback(&Messenger::callback)
                      .setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
                      .setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
              );

              VkSurfaceKHR vkSurface = nullptr;
              auto vkInstance = static_cast<VkInstance>(*(*instance->value));
              if (glfwCreateWindowSurface(vkInstance, window, nullptr, &vkSurface) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to create surface!");
              }
              if (vkSurface == nullptr) {
                throw std::runtime_error(CALL_INFO() + ": surface is null!");
              }
              surface = environment->createSurface(*instance, vkSurface);

              physicalDevice = environment->createPhysicalDevice(
                  *instance,
                  context->createInfo.enabledDeviceExtensionNames,
                  vk::PhysicalDeviceFeatures()
                    .setSamplerAnisotropy(context->createInfo.samplerAnisotropy),
                  1.0f,
                  {vk::QueueFlagBits::eCompute, vk::QueueFlagBits::eTransfer, vk::QueueFlagBits::eGraphics},
                  surface
              );

              device = environment->createDevice(
                  *physicalDevice,
                  vk::DeviceCreateInfo()
                      .setQueueCreateInfos(physicalDevice->uniqueQueueCreateInfos)
                      .setPEnabledFeatures(&physicalDevice->features)
                      .setPEnabledExtensionNames(context->createInfo.enabledDeviceExtensionNames)
                      .setPEnabledLayerNames(context->createInfo.enabledLayerNames)
              );

              swapChain = environment->createSwapChain(
                  *device,
                  environment->swapChainCreateInfo(*physicalDevice, *surface, width, height)
              );

              std::cout << std::format("context->createInfo.environmentVariables['VK_LAYER_PATH']: '{}'", context->createInfo.environmentVariables["VK_LAYER_PATH"]) << std::endl;
              std::cout << std::format("context->id: '{}'", context->id) << std::endl;
              std::cout << std::format("instance->id: '{}'", instance->id) << std::endl;
              std::cout << std::format("messenger->id: '{}'", messenger->id) << std::endl;
              std::cout << std::format("debugUtilsMessenger->id: '{}'", debugUtilsMessenger->id) << std::endl;
              std::cout << std::format("surface->id: '{}'", surface->id) << std::endl;
              std::cout << std::format("physicalDevice->id: '{}'", physicalDevice->id) << std::endl;
              std::cout << std::format("device->id: '{}'", device->id) << std::endl;
              std::cout << std::format("swapChain->id: '{}'", swapChain->id) << std::endl;
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void drawFrame(int width, int height) {
            try {
              // TODO
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void waitIdle() {
            try {
              // TODO
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void destroy() {
            try {
              // TODO
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

      };

      class TestUiApplication {

        public:

          std::vector<std::string> arguments = {};
          Renderer* renderer = nullptr;

          TestUiApplication(const int& argc, char** argv) {
            try {
              for (std::size_t i = 0; i < argc; i++) {
                arguments.emplace_back(std::string(argv[i]));
              }
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          int run() {
            try {
              int width = 800;
              int height = 600;
              std::string title = "Vulkan";

              glfwInit();
              glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
              //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

              GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

              glfwSetWindowUserPointer(window, this);
              glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

              renderer = new Renderer();
              renderer->create(arguments, window, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

              while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
                glfwGetFramebufferSize(window, &width, &height);
                while (width == 0 || height == 0) {
                  glfwGetFramebufferSize(window, &width, &height);
                  glfwWaitEvents();
                }
                renderer->drawFrame(width, height);
              }
              renderer->waitIdle();

              renderer->destroy();
              delete renderer;
              glfwDestroyWindow(window);
              glfwTerminate();

              return EXIT_SUCCESS;
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

        private:

          static void frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
            try {
              auto* app = reinterpret_cast<TestUiApplication*>(glfwGetWindowUserPointer(window));
              app->renderer->resized = true;
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

      };

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

  TEST_F(UiTestsA, test1) {
    try {
      std::string executableDir = TestUtils::getExecutableDir();
      std::vector<char*> arguments = {executableDir.data()};
      int argc = static_cast<int>(arguments.size());
      char** argv = &arguments[0];
      int result = TestUiApplication(argc, argv).run();
      ASSERT_EQ(EXIT_SUCCESS, result);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
