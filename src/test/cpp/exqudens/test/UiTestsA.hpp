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
#include "exqudens/vulkan/Factory.hpp"

namespace exqudens::vulkan {

  class UiTestsA : public testing::Test {

    protected:

      class Environment : public Factory {

        public:

          std::map<std::string, std::string> environmentVariables = {};
          Configuration configuration = {};
          Logger logger = {};
          VkInstance instance = nullptr;
          VkDebugUtilsMessengerEXT debugUtilsMessenger = nullptr;
          VkSurfaceKHR surface = nullptr;
          PhysicalDevice physicalDevice = {};
          VkDevice device = nullptr;
          //VkQueue computeQueue = nullptr;
          //VkQueue transferQueue = nullptr;
          Queue graphicsQueue = {};
          Queue presentQueue = {};
          SwapChain swapChain = {};
          std::vector<VkImage> swapChainImages = {};
          std::vector<VkImageView> swapChainImageViews = {};
          VkRenderPass renderPass = nullptr;
          //VkDescriptorSetLayout descriptorSetLayout = nullptr;
          Pipeline graphicsPipeline = {};
          std::vector<VkFramebuffer> swapChainFrameBuffers = {};
          //VkDescriptorPool descriptorPool = nullptr;
          //VkCommandPool transferCommandPool = nullptr;
          VkCommandPool graphicsCommandPool = nullptr;
          //std::vector<VkCommandBuffer> transferCommandBuffers = {};
          std::vector<VkCommandBuffer> graphicsCommandBuffers = {};

          std::vector<VkSemaphore> imageAvailableSemaphores = {};
          std::vector<VkSemaphore> renderFinishedSemaphores = {};
          std::vector<VkFence> inFlightFences = {};
          std::vector<VkFence> imagesInFlight = {};

          size_t currentFrame = 0;
          int MAX_FRAMES_IN_FLIGHT = 2;

          bool resized = false;

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

              environmentVariables = createEnvironmentVariables(TestConfiguration::getExecutableDir());

              for (auto const& [name, value] : environmentVariables) {
                setEnvironmentVariable(name, value);
              }

              configuration = createConfiguration();
              for (const char*& extension : extensions) {
                configuration.extensions.emplace_back(extension);
              }
              configuration.anisotropyRequired = false;
              logger = createLogger();

              functions = createFunctions();
              functions.createSurfaceKHR = [&window](VkInstance i) -> VkSurfaceKHR {
                VkSurfaceKHR result;
                if (glfwCreateWindowSurface(i, window, nullptr, &result) != VK_SUCCESS) {
                  throw std::runtime_error(CALL_INFO() + ": failed to create window surface!");
                }
                return result;
              };

              instance = createInstance(configuration, logger);
              debugUtilsMessenger = createDebugUtilsMessenger(instance, logger);
              surface = createSurface(instance);
              physicalDevice = createPhysicalDevice(instance, configuration, surface);
              device = createDevice(physicalDevice.value, configuration, physicalDevice.queueFamilyIndexInfo);
              //computeQueue = createComputeQueue(physicalDevice, configuration, surface, device, 0);
              //transferQueue = createTransferQueue(physicalDevice, configuration, surface, device, 0);
              graphicsQueue = createQueue(device, physicalDevice.queueFamilyIndexInfo.graphicsFamily.value(), 0);
              presentQueue = createQueue(device, physicalDevice.queueFamilyIndexInfo.presentFamily.value(), 0);
              graphicsCommandPool = createCommandPool(device, graphicsQueue.familyIndex);
              swapChain = createSwapChain(physicalDevice.swapChainSupportDetails.value(), physicalDevice.queueFamilyIndexInfo, surface, device, 800, 600);
              swapChainImages = createSwapChainImages(device, swapChain.value);
              swapChainImageViews = createImageViews(device, swapChainImages, swapChain.format);
              renderPass = createRenderPass(device, swapChain.format);
              graphicsPipeline = createGraphicsPipeline(device, swapChain.extent, {"resources/shader/shader-1.vert.spv", "resources/shader/shader-1.frag.spv"}, renderPass);
              std::vector<FrameBufferCreateInfo> frameBufferCreateInfoVector;
              frameBufferCreateInfoVector.resize(swapChainImageViews.size());
              for (std::size_t i = 0; i < frameBufferCreateInfoVector.size(); i++) {
                frameBufferCreateInfoVector[i] = FrameBufferCreateInfo {
                    .flags = 0,
                    .renderPass = renderPass,
                    .attachments = {
                        swapChainImageViews[i]
                    },
                    .width = swapChain.extent.width,
                    .height = swapChain.extent.height,
                    .layers = 1
                };
              }
              swapChainFrameBuffers = createFrameBuffers(device, frameBufferCreateInfoVector);
              graphicsCommandBuffers = createCommandBuffers(device, graphicsCommandPool, swapChainImageViews.size());

              fillCommandBuffers(
                  graphicsCommandBuffers,
                  renderPass,
                  swapChainFrameBuffers,
                  swapChain.extent,
                  graphicsPipeline.value
              );

              imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
              renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
              inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
              imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);

              VkSemaphoreCreateInfo semaphoreInfo{};
              semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

              VkFenceCreateInfo fenceInfo{};
              fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
              fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

              for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                if (
                    vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS
                    || vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS
                    || vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS
                ) {
                  throw std::runtime_error(CALL_INFO() + ": failed to create synchronization objects for a frame!");
                }
              }
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void fillCommandBuffers(
              std::vector<VkCommandBuffer>& commandBuffers,
              VkRenderPass& renderPass,
              std::vector<VkFramebuffer> frameBuffers,
              VkExtent2D& extent,
              VkPipeline& pipeline
          ) {
            for (size_t i = 0; i < commandBuffers.size(); i++) {
              VkCommandBufferBeginInfo beginInfo{};
              beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

              if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to begin recording command buffer!");
              }

              VkRenderPassBeginInfo renderPassInfo{};
              renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
              renderPassInfo.renderPass = renderPass;
              renderPassInfo.framebuffer = frameBuffers[i];
              renderPassInfo.renderArea.offset = {0, 0};
              renderPassInfo.renderArea.extent = extent;

              VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
              renderPassInfo.clearValueCount = 1;
              renderPassInfo.pClearValues = &clearColor;

              vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

              vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

              vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

              vkCmdEndRenderPass(commandBuffers[i]);

              if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to record command buffer!");
              }
            }
          }

          void reCreateSwapChain(int width, int height) {
            std::cout << __FUNCTION__ << " width: " << width << " height: " << height << std::endl;

            vkDeviceWaitIdle(device);

            destroyCommandBuffers(graphicsCommandBuffers, graphicsCommandPool, device);
            destroyFrameBuffers(swapChainFrameBuffers, device);
            destroyPipeline(graphicsPipeline, device);
            destroyRenderPass(renderPass, device);
            destroyImageViews(swapChainImageViews, device);
            destroySwapChain(swapChain, device);

            physicalDevice.swapChainSupportDetails = querySwapChainSupport(physicalDevice.value, surface);

            swapChain = createSwapChain(physicalDevice.swapChainSupportDetails.value(), physicalDevice.queueFamilyIndexInfo, surface, device, width, height);
            swapChainImages = createSwapChainImages(device, swapChain.value);
            swapChainImageViews = createImageViews(device, swapChainImages, swapChain.format);
            renderPass = createRenderPass(device, swapChain.format);
            graphicsPipeline = createGraphicsPipeline(device, swapChain.extent, {"resources/shader/shader.vert.spv", "resources/shader/shader.frag.spv"}, renderPass);
            std::vector<FrameBufferCreateInfo> frameBufferCreateInfoVector;
            frameBufferCreateInfoVector.resize(swapChainImageViews.size());
            for (std::size_t i = 0; i < frameBufferCreateInfoVector.size(); i++) {
              frameBufferCreateInfoVector[i] = FrameBufferCreateInfo {
                  .flags = 0,
                  .renderPass = renderPass,
                  .attachments = {
                      swapChainImageViews[i]
                  },
                  .width = swapChain.extent.width,
                  .height = swapChain.extent.height,
                  .layers = 1
              };
            }
            swapChainFrameBuffers = createFrameBuffers(device, frameBufferCreateInfoVector);
            graphicsCommandBuffers = createCommandBuffers(device, graphicsCommandPool, swapChainImageViews.size());

            fillCommandBuffers(
                graphicsCommandBuffers,
                renderPass,
                swapChainFrameBuffers,
                swapChain.extent,
                graphicsPipeline.value
            );
          }

          void drawFrame(int width, int height) {
            try {
              vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

              uint32_t imageIndex;
              VkResult result = vkAcquireNextImageKHR(device, swapChain.value, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

              if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                reCreateSwapChain(width, height);
                return;
              } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                throw std::runtime_error("failed to acquire swap chain image!");
              }

              if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
                vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
              }
              imagesInFlight[imageIndex] = inFlightFences[currentFrame];

              VkSubmitInfo submitInfo{};
              submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

              VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
              VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
              submitInfo.waitSemaphoreCount = 1;
              submitInfo.pWaitSemaphores = waitSemaphores;
              submitInfo.pWaitDstStageMask = waitStages;

              submitInfo.commandBufferCount = 1;
              submitInfo.pCommandBuffers = &graphicsCommandBuffers[imageIndex];

              VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
              submitInfo.signalSemaphoreCount = 1;
              submitInfo.pSignalSemaphores = signalSemaphores;

              vkResetFences(device, 1, &inFlightFences[currentFrame]);

              if (vkQueueSubmit(graphicsQueue.value, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
                throw std::runtime_error("failed to submit draw command buffer!");
              }

              VkPresentInfoKHR presentInfo{};
              presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

              presentInfo.waitSemaphoreCount = 1;
              presentInfo.pWaitSemaphores = signalSemaphores;

              VkSwapchainKHR swapChains[] = {swapChain.value};
              presentInfo.swapchainCount = 1;
              presentInfo.pSwapchains = swapChains;

              presentInfo.pImageIndices = &imageIndex;

              result = vkQueuePresentKHR(presentQueue.value, &presentInfo);

              if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || resized) {
                resized = false;
                reCreateSwapChain(width, height);
              } else if (result != VK_SUCCESS) {
                throw std::runtime_error("failed to present swap chain image!");
              }

              currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void waitIdle() {
            try {
              vkDeviceWaitIdle(device);
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void destroy() {
            try {
              for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
                vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
                vkDestroyFence(device, inFlightFences[i], nullptr);
              }

              destroyCommandBuffers(graphicsCommandBuffers, graphicsCommandPool, device);
              destroyFrameBuffers(swapChainFrameBuffers, device);
              destroyPipeline(graphicsPipeline, device);
              destroyRenderPass(renderPass, device);
              destroyImageViews(swapChainImageViews, device);
              destroySwapChain(swapChain, device);
              destroyCommandPool(graphicsCommandPool, device);
              destroyQueue(presentQueue);
              destroyQueue(graphicsQueue);
              destroyDevice(device);
              destroyPhysicalDevice(physicalDevice);
              destroySurface(surface, instance);
              destroyDebugUtilsMessenger(debugUtilsMessenger, instance);
              destroyInstance(instance);
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

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
              //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

              window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

              glfwSetWindowUserPointer(window, this);
              glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

              environment = new Environment();
              environment->create(window);

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

  };

  TEST_F(UiTestsA, test1) {
    try {
      int argc = 0;
      char** argv = nullptr;
      int result = TestUiApplication(argc, argv).run();
      ASSERT_EQ(EXIT_SUCCESS, result);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
