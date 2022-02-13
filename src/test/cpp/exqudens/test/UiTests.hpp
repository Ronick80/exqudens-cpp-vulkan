#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

#include "exqudens/TestMacros.hpp"
#include "exqudens/TestConfiguration.hpp"
#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/utility/Factory.hpp"

namespace exqudens::vulkan {

  class UiTests : public testing::Test {

    protected:

      class Environment : public Factory {

        public:

          std::map<std::string, std::string> environmentVariables = {};
          Configuration configuration = {};
          Logger logger = {};
          VkInstance instance = nullptr;
          VkDebugUtilsMessengerEXT debugUtilsMessenger = nullptr;
          VkSurfaceKHR surface = nullptr;
          VkPhysicalDevice physicalDevice = nullptr;
          VkDevice device = nullptr;
          VkQueue computeQueue = nullptr;
          VkQueue transferQueue = nullptr;
          VkQueue graphicsQueue = nullptr;
          VkQueue presentQueue = nullptr;
          SwapChain swapChain = {};
          std::vector<VkImage> swapChainImages = {};
          std::vector<VkImageView> swapChainImageViews = {};
          VkRenderPass renderPass = nullptr;
          VkDescriptorSetLayout descriptorSetLayout = nullptr;
          Pipeline graphicsPipeline = {};

          Environment() = default;

          void create(GLFWwindow*& window) {
            try {
              uint32_t glfwExtensionCount = 0;
              const char** glfwExtensions;
              glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
              std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

              int width = 0;
              int height = 0;
              glfwGetFramebufferSize(window, &width, &height);

              createSurfaceFunction = [&window](VkInstance& i) -> VkSurfaceKHR {
                VkSurfaceKHR result;
                if (glfwCreateWindowSurface(i, window, nullptr, &result) != VK_SUCCESS) {
                  throw std::runtime_error(CALL_INFO() + ": failed to create window surface!");
                }
                return result;
              };

              environmentVariables = createEnvironmentVariables(TestConfiguration::getExecutableDir());

              for (auto const& [name, value] : environmentVariables) {
                setEnvironmentVariable(name, value);
              }

              configuration = createConfiguration();
              for (const char*& extension : extensions) {
                configuration.extensions.emplace_back(extension);
              }
              logger = createLogger();

              instance = createInstance(configuration, logger);
              debugUtilsMessenger = createDebugUtilsMessenger(instance, logger);
              surface = createSurface(instance);
              physicalDevice = createPhysicalDevice(instance, configuration, surface);
              device = createDevice(physicalDevice, configuration, surface);
              computeQueue = createComputeQueue(physicalDevice, configuration, surface, device, 0);
              transferQueue = createTransferQueue(physicalDevice, configuration, surface, device, 0);
              graphicsQueue = createGraphicsQueue(physicalDevice, configuration, surface, device, 0);
              presentQueue = createPresentQueue(physicalDevice, configuration, surface, device, 0);
              swapChain = createSwapChain(physicalDevice, configuration, surface, device, 800, 600);
              swapChainImages = createSwapChainImages(physicalDevice, surface, device, swapChain.value);
              swapChainImageViews = createImageViews(device, swapChainImages, swapChain.format);
              renderPass = createRenderPass(device, swapChain.format);
              descriptorSetLayout = createDescriptorSetLayout(device);
              graphicsPipeline = createGraphicsPipeline(device, swapChain.extent, descriptorSetLayout, {"resources/shader/shader.vert.spv", "resources/shader/shader.frag.spv"}, renderPass);
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void drawFrame(int width, int height) {
            try {
              //
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void waitIdle() {
            try {
              //
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void destroy() {
            try {
              destroyPipeline(graphicsPipeline, device);
              destroyDescriptorSetLayout(descriptorSetLayout, device);
              destroyRenderPass(renderPass, device);
              destroyImageViews(swapChainImageViews, device);
              destroySwapChain(swapChain, device);
              destroyDevice(device);
              destroyPhysicalDevice(physicalDevice);
              destroySurface(surface, instance);
              destroyDebugUtilsMessenger(debugUtilsMessenger, instance);
              destroyInstance(instance);
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          ~Environment() override = default;

      };

      class TestUiApplication {

        public:

          std::vector<std::string> arguments = {};
          GLFWwindow* window = nullptr;
          Environment* environment = nullptr;

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

              window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

              glfwSetWindowUserPointer(window, this);
              glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

              environment = new Environment();
              environment->create(window);

              while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
                int width = 0;
                int height = 0;
                glfwGetFramebufferSize(window, &width, &height);
                while (width == 0 || height == 0) {
                  glfwGetFramebufferSize(window, &width, &height);
                  glfwWaitEvents();
                }
                environment->drawFrame(width, height);
              }
              environment->waitIdle();

              environment->destroy();
              delete environment;
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
              //TestUiApplication* app = reinterpret_cast<TestUiApplication*>(glfwGetWindowUserPointer(window));
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

      };

  };

  TEST_F(UiTests, test1) {
    try {
      int argc = 0;
      char** argv = nullptr;
      int result = TestUiApplication(argc, argv).run();
      ASSERT_EQ(EXIT_SUCCESS, result);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

  TEST_F(UiTests, test2) {
    try {
      ASSERT_EQ(1, 2);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
