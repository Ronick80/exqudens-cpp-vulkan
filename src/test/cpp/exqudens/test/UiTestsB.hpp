#pragma once

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

#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/raii/Utility.hpp"

namespace exqudens::vulkan {

  class UiTestsB : public testing::Test {

    protected:

      class Environment {

        public:

          bool resized = false;

        private:

          std::vector<const char*> enabledLayerNames = {};
          std::vector<const char*> enabledExtensionNames = {};
          std::vector<const char*> enabledDeviceExtensionNames = {};

          std::optional<vk::raii::Context> context = {};
          std::optional<vk::raii::Instance> instance = {};
          std::optional<vk::raii::DebugUtilsMessengerEXT> debugUtilsMessenger = {};
          std::optional<vk::raii::SurfaceKHR> surface = {};

          std::vector<vk::raii::PhysicalDevice> physicalDevices = {};
          std::optional<vk::PhysicalDeviceFeatures> physicalDeviceEnabledFeatures = {};
          vk::raii::PhysicalDevice* physicalDevice = nullptr;

          std::optional<std::uint32_t> transferQueueFamilyIndex = {};
          std::optional<std::uint32_t> graphicsQueueFamilyIndex = {};
          std::optional<std::uint32_t> presentQueueFamilyIndex = {};

          std::optional<vk::raii::Device> device = {};

          std::optional<vk::raii::SwapchainKHR> swapChain = {};

          std::optional<vk::raii::CommandPool> transferCommandPool = {};
          std::vector<vk::raii::CommandBuffer> transferCommandBuffers = {};
          std::optional<vk::raii::Queue> transferQueue = {};

          std::optional<vk::raii::CommandPool> graphicsCommandPool = {};
          std::vector<vk::raii::CommandBuffer> graphicsCommandBuffers = {};
          std::optional<vk::raii::Queue> graphicsQueue = {};

        public:

          void create(const std::vector<std::string>& arguments, GLFWwindow* window, uint32_t width, uint32_t height) {
            try {
              TestUtils::setEnvironmentVariable("VK_LAYER_PATH", arguments.front());

              enabledLayerNames = {"VK_LAYER_KHRONOS_validation"};
              enabledExtensionNames = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
              enabledDeviceExtensionNames = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

              uint32_t glfwExtensionCount = 0;
              const char** glfwExtensions;
              glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
              std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
              for (const char*& extension : extensions) {
                enabledExtensionNames.emplace_back(extension);
              }

              context = vk::raii::Context();

              vk::ApplicationInfo applicationInfo = vk::ApplicationInfo()
                  .setPApplicationName("Exqudens Application")
                  .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
                  .setPEngineName("Exqudens Engine")
                  .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
                  .setApiVersion(VK_API_VERSION_1_0);
              instance = vk::raii::Instance(
                  *context,
                  vk::InstanceCreateInfo()
                      .setPApplicationInfo(&applicationInfo)
                      .setPEnabledExtensionNames(enabledExtensionNames)
                      .setPEnabledLayerNames(enabledLayerNames)
              );

              debugUtilsMessenger = vk::raii::DebugUtilsMessengerEXT(
                  *instance,
                  vk::DebugUtilsMessengerCreateInfoEXT()
                      .setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
                      .setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
                      .setPfnUserCallback(&Environment::debugCallback)
                      .setPUserData(nullptr)
              );

              VkSurfaceKHR vkSurface = nullptr;
              auto vkInstance = static_cast<VkInstance>(*instance.value());
              if (glfwCreateWindowSurface(vkInstance, window, nullptr, &vkSurface) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to create surface!");
              }
              if (vkSurface == nullptr) {
                throw std::runtime_error(CALL_INFO() + ": surface is null!");
              }
              surface = vk::raii::SurfaceKHR(*instance, vkSurface);

              physicalDevices = vk::raii::PhysicalDevices(*instance);
              physicalDeviceEnabledFeatures = vk::PhysicalDeviceFeatures()
                  .setSamplerAnisotropy(true);
              std::size_t physicalDeviceIndex = raii::Utility::getPhysicalDeviceIndices(
                  physicalDevices,
                  {vk::QueueFlagBits::eCompute, vk::QueueFlagBits::eTransfer, vk::QueueFlagBits::eGraphics},
                  &surface.value(),
                  enabledDeviceExtensionNames,
                  (*physicalDeviceEnabledFeatures).samplerAnisotropy
              ).front();
              physicalDevice = &physicalDevices.at(physicalDeviceIndex);

              transferQueueFamilyIndex = raii::Utility::getQueueFamilyIndices(
                  *physicalDevice,
                  vk::QueueFlagBits::eTransfer,
                  nullptr
              ).front();
              graphicsQueueFamilyIndex = raii::Utility::getQueueFamilyIndices(
                  *physicalDevice,
                  vk::QueueFlagBits::eGraphics,
                  nullptr
              ).front();
              presentQueueFamilyIndex = raii::Utility::getQueueFamilyIndices(
                  *physicalDevice,
                  {},
                  &surface.value()
              ).front();

              std::set<std::uint32_t> uniqueQueueFamilyIndices = {
                  *transferQueueFamilyIndex,
                  *graphicsQueueFamilyIndex,
                  *presentQueueFamilyIndex
              };
              std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
              float queuePriority = 1.0f;
              for (const uint32_t& queueFamilyIndex : uniqueQueueFamilyIndices) {
                vk::DeviceQueueCreateInfo info = vk::DeviceQueueCreateInfo()
                    .setQueueFamilyIndex(queueFamilyIndex)
                    .setQueueCount(1)
                    .setQueuePriorities(queuePriority);
                queueCreateInfo.emplace_back(info);
              }
              device = vk::raii::Device(
                  *physicalDevice,
                  vk::DeviceCreateInfo()
                      .setQueueCreateInfos(queueCreateInfo)
                      .setPEnabledFeatures(&physicalDeviceEnabledFeatures.value())
                      .setPEnabledExtensionNames(enabledDeviceExtensionNames)
                      .setPEnabledLayerNames(enabledLayerNames)
              );

              vk::SurfaceFormatKHR surfaceFormat = raii::Utility::surfaceFormat(physicalDevice->getSurfaceFormatsKHR(*surface.value())).value();
              vk::PresentModeKHR surfacePresentMode = raii::Utility::surfacePresentMode(physicalDevice->getSurfacePresentModesKHR(*surface.value())).value();
              vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice->getSurfaceCapabilitiesKHR(*surface.value());
              vk::Extent2D surfaceExtent = raii::Utility::surfaceExtent(surfaceCapabilities, width, height).value();
              vk::SurfaceTransformFlagBitsKHR surfaceTransform = raii::Utility::surfaceTransform(surfaceCapabilities).value();
              vk::CompositeAlphaFlagBitsKHR surfaceCompositeAlpha = raii::Utility::surfaceCompositeAlpha(surfaceCapabilities).value();
              swapChain = vk::raii::SwapchainKHR(
                  *device,
                  vk::SwapchainCreateInfoKHR()
                    .setFlags({})
                    .setSurface(*surface.value())
                    .setMinImageCount(surfaceCapabilities.minImageCount)
                    .setImageFormat(surfaceFormat.format)
                    .setImageColorSpace(surfaceFormat.colorSpace)
                    .setImageExtent(surfaceExtent)
                    .setImageArrayLayers(1)
                    .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
                    .setImageSharingMode(vk::SharingMode::eExclusive)
                    .setQueueFamilyIndices({})
                    .setPreTransform(surfaceTransform)
                    .setCompositeAlpha(surfaceCompositeAlpha)
                    .setPresentMode(surfacePresentMode)
                    .setClipped(true)
                    .setOldSwapchain({})
              );

              transferCommandPool = vk::raii::CommandPool(
                  *device,
                  vk::CommandPoolCreateInfo()
                    .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                    .setQueueFamilyIndex(*transferQueueFamilyIndex)
              );
              transferCommandBuffers = vk::raii::CommandBuffers(
                  *device,
                  vk::CommandBufferAllocateInfo()
                    .setCommandPool(*transferCommandPool.value())
                    .setLevel(vk::CommandBufferLevel::ePrimary)
                    .setCommandBufferCount(1)
              );
              transferQueue = vk::raii::Queue(*device, *transferQueueFamilyIndex, 0);

              graphicsCommandPool = vk::raii::CommandPool(
                  *device,
                  vk::CommandPoolCreateInfo()
                      .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                      .setQueueFamilyIndex(*transferQueueFamilyIndex)
              );
              graphicsCommandBuffers = vk::raii::CommandBuffers(
                  *device,
                  vk::CommandBufferAllocateInfo()
                      .setCommandPool(*graphicsCommandPool.value())
                      .setLevel(vk::CommandBufferLevel::ePrimary)
                      .setCommandBufferCount(1)
              );
              graphicsQueue = vk::raii::Queue(*device, *graphicsQueueFamilyIndex, 0);

              // TODO
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

        private:

          static VkBool32 debugCallback(
              VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageTypes,
              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
              void* pUserData
          ) {
            try {
              std::string level;
              if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT == messageSeverity) {
                level = "[VERBOSE]";
              } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT == messageSeverity) {
                level = "[INFO]";
              } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT == messageSeverity) {
                level = "[WARNING]";
              } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT == messageSeverity) {
                level = "[ERROR]";
              } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT == messageSeverity) {
                level = "[MAX]";
              }

              std::string type;
              if (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT == messageTypes) {
                type = "(GENERAL)";
              } else if (VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT == messageTypes) {
                type = "(VALIDATION)";
              } else if (VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT == messageTypes) {
                type = "(PERFORMANCE)";
              } else if (VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT == messageTypes) {
                type = "(MAX)";
              }

              std::string message(pCallbackData->pMessage);

              std::string line;

              line += level;
              line += " ";
              line += type;
              line += " ";
              line += "validation layer:";
              line += " ";
              line += message;

              std::cout << std::format("{}", CALL_INFO() + ": " + line) << std::endl;
              return VK_FALSE;
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

      };

      class TestUiApplication {

        public:

          std::vector<std::string> arguments = {};
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
              //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

              GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

              glfwSetWindowUserPointer(window, this);
              glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

              environment = new Environment();
              environment->create(arguments, window, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

              while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
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
              auto* app = reinterpret_cast<TestUiApplication*>(glfwGetWindowUserPointer(window));
              app->environment->resized = true;
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

  TEST_F(UiTestsB, test1) {
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
