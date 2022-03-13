#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include <chrono>
#include <filesystem>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <gtest/gtest.h>

#include "exqudens/TestMacros.hpp"
#include "exqudens/TestConfiguration.hpp"
#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/Factory.hpp"
#include "exqudens/test/model/Vertex.hpp"
#include "exqudens/test/model/UniformBufferObject.hpp"

namespace exqudens::vulkan {

  class UiTestsD : public testing::Test {

    protected:

      class Environment : public Factory {

        public:

          std::vector<Vertex> vertices = {};
          std::vector<uint16_t> indices = {};

          std::map<std::string, std::string> environmentVariables = {};
          Configuration configuration = {};
          Logger logger = {};
          VkInstance instance = nullptr;
          VkDebugUtilsMessengerEXT debugUtilsMessenger = nullptr;
          VkSurfaceKHR surface = nullptr;
          PhysicalDevice physicalDevice = {};
          VkDevice device = nullptr;
          Queue transferQueue = {};
          Queue graphicsQueue = {};
          Queue presentQueue = {};
          SwapChain swapChain = {};
          std::vector<VkImage> swapChainImages = {};
          std::vector<VkImageView> swapChainImageViews = {};
          Image depthImage = {};
          VkImageView depthImageView = {};
          VkRenderPass renderPass = nullptr;
          VkDescriptorSetLayout descriptorSetLayout = nullptr;
          Pipeline graphicsPipeline = {};
          std::vector<VkFramebuffer> swapChainFrameBuffers = {};
          VkCommandPool transferCommandPool = nullptr;
          VkCommandPool graphicsCommandPool = nullptr;
          Buffer imageStaging = {};
          Image image = {};
          VkImageView imageView = nullptr;
          Buffer vertexStagingBuffer = {};
          Buffer vertexBuffer = {};
          Buffer indexStagingBuffer = {};
          Buffer indexBuffer = {};
          std::vector<Buffer> uniformBuffers = {};
          VkSampler sampler = nullptr;
          VkDescriptorPool descriptorPool = nullptr;
          std::vector<VkDescriptorSet> descriptorSets = {};
          VkCommandBuffer transferCommandBuffer = nullptr;
          std::vector<VkCommandBuffer> graphicsCommandBuffers = {};

          std::vector<VkSemaphore> imageAvailableSemaphores = {};
          std::vector<VkSemaphore> renderFinishedSemaphores = {};
          std::vector<VkFence> inFlightFences = {};

          std::size_t currentFrame = 0;
          int MAX_FRAMES_IN_FLIGHT = 2;

          bool resized = false;

          void create(GLFWwindow*& window) {
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
              logger = createLogger(
                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
              );

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
              transferQueue = createQueue(device, physicalDevice.queueFamilyIndexInfo.transferFamily.value(), 0);
              graphicsQueue = createQueue(device, physicalDevice.queueFamilyIndexInfo.graphicsFamily.value(), 0);
              presentQueue = createQueue(device, physicalDevice.queueFamilyIndexInfo.presentFamily.value(), 0);
              transferCommandPool = createCommandPool(device, transferQueue.familyIndex, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
              graphicsCommandPool = createCommandPool(device, graphicsQueue.familyIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
              swapChain = createSwapChain(physicalDevice.swapChainSupportDetails.value(), physicalDevice.queueFamilyIndexInfo, surface, device, 800, 600);
              swapChainImages = createSwapChainImages(device, swapChain.value);
              swapChainImageViews = createImageViews(device, swapChainImages, swapChain.format);

              depthImage = createImage(
                  physicalDevice.value,
                  device,
                  swapChain.width,
                  swapChain.height,
                  findDepthFormat(physicalDevice.value),
                  VK_IMAGE_TILING_OPTIMAL,
                  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
              );
              depthImageView = createImageView(device, depthImage.value, depthImage.format, VK_IMAGE_ASPECT_DEPTH_BIT);
              transitionImageLayout(device, transferQueue.value, transferCommandPool, depthImage.value, depthImage.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

              renderPass = createRenderPass(
                  device,
                  RenderPassCreateInfo {
                      .attachments = {
                          VkAttachmentDescription {
                              .format = swapChain.format,
                              .samples = VK_SAMPLE_COUNT_1_BIT,
                              .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                              .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                              .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                              .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                              .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                              .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
                          },
                          VkAttachmentDescription {
                              .format = depthImage.format,
                              .samples = VK_SAMPLE_COUNT_1_BIT,
                              .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                              .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                              .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                              .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                              .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                              .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                          }
                      },
                      .subPasses = {
                          SubPassDescription {
                              .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                              .colorAttachments = {
                                  VkAttachmentReference {
                                      .attachment = 0,
                                      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                                  }
                              },
                              .depthStencilAttachment = VkAttachmentReference {
                                  .attachment = 1,
                                  .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                              }
                          }
                      },
                      .dependencies = {
                          VkSubpassDependency {
                              .srcSubpass = VK_SUBPASS_EXTERNAL,
                              .dstSubpass = 0,
                              .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                              .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                              .srcAccessMask = 0,
                              .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
                          }
                      }
                  }
              );
              descriptorSetLayout = createDescriptorSetLayout(
                  device,
                  DescriptorSetLayoutCreateInfo {
                    .flags = 0,
                    .bindings = {
                        VkDescriptorSetLayoutBinding {
                          .binding = 0,
                          .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                          .descriptorCount = 1,
                          .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                          .pImmutableSamplers = nullptr
                        },
                        VkDescriptorSetLayoutBinding {
                            .binding = 1,
                            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                            .descriptorCount = 1,
                            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                            .pImmutableSamplers = nullptr
                        }
                    }
                  }
              );
              graphicsPipeline = createGraphicsPipeline(
                  device,
                  swapChain.extent,
                  {"resources/shader/shader-4.vert.spv", "resources/shader/shader-4.frag.spv"},
                  renderPass,
                  VK_FRONT_FACE_COUNTER_CLOCKWISE,
                  {descriptorSetLayout},
                  {Vertex::getBindingDescription()},
                  Vertex::getAttributeDescriptions()
              );

              unsigned int imageWidth, imageHeight, imageDepth;
              std::vector<unsigned char> pixels;
              TestUtils::readPng(
                  std::filesystem::path().append("resources").append("png").append("texture.png").make_preferred().string(),
                  imageWidth,
                  imageHeight,
                  imageDepth,
                  pixels
              );

              imageStaging = createBuffer(physicalDevice.value, device, imageWidth * imageHeight * imageDepth, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

              void* imageData;
              vkMapMemory(device, imageStaging.memory, 0, imageStaging.memorySize, 0, &imageData);
              std::memcpy(imageData, pixels.data(), static_cast<size_t>(imageStaging.memorySize));
              vkUnmapMemory(device, imageStaging.memory);

              std::vector<FrameBufferCreateInfo> frameBufferCreateInfoVector;
              frameBufferCreateInfoVector.resize(swapChainImageViews.size());
              for (std::size_t i = 0; i < frameBufferCreateInfoVector.size(); i++) {
                frameBufferCreateInfoVector[i] = FrameBufferCreateInfo {
                    .flags = 0,
                    .renderPass = renderPass,
                    .attachments = {
                        swapChainImageViews[i],
                        depthImageView
                    },
                    .width = swapChain.extent.width,
                    .height = swapChain.extent.height,
                    .layers = 1
                };
              }
              swapChainFrameBuffers = createFrameBuffers(device, frameBufferCreateInfoVector);

              image = createImage(physicalDevice.value, device, imageWidth, imageHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
              imageView = createImageView(device, image.value, VK_FORMAT_R8G8B8A8_SRGB);
              vertexStagingBuffer = createBuffer(physicalDevice.value, device, sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

              void* vertexData;
              vkMapMemory(device, vertexStagingBuffer.memory, 0, vertexStagingBuffer.memorySize, 0, &vertexData);
              std::memcpy(vertexData, vertices.data(), (size_t) vertexStagingBuffer.memorySize);
              vkUnmapMemory(device, vertexStagingBuffer.memory);

              vertexBuffer = createBuffer(physicalDevice.value, device, sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
              indexStagingBuffer = createBuffer(physicalDevice.value, device, sizeof(indices[0]) * indices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

              void* indexData;
              vkMapMemory(device, indexStagingBuffer.memory, 0, indexStagingBuffer.memorySize, 0, &indexData);
              std::memcpy(indexData, indices.data(), (size_t) indexStagingBuffer.memorySize);
              vkUnmapMemory(device, indexStagingBuffer.memory);

              indexBuffer = createBuffer(physicalDevice.value, device, sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
              uniformBuffers = createBuffers(physicalDevice.value, device, sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, MAX_FRAMES_IN_FLIGHT);
              sampler = createSampler(physicalDevice.value, device);
              descriptorPool = createDescriptorPool(
                  device,
                  DescriptorPoolCreateInfo {
                      .maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
                      .poolSizes = {
                          VkDescriptorPoolSize {
                              .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                              .descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)
                          },
                          VkDescriptorPoolSize {
                              .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                              .descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)
                          }
                      }
                  }
              );
              descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
              for (std::size_t i = 0; i < descriptorSets.size(); i++) {
                descriptorSets[i] = createDescriptorSet(
                    device,
                    descriptorPool,
                    descriptorSetLayout,
                    {
                        WriteDescriptorSet {
                            .dstBinding = 0,
                            .dstArrayElement = 0,
                            .descriptorCount = 1,
                            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                            .imageInfo = {},
                            .bufferInfo = {
                                VkDescriptorBufferInfo {
                                    .buffer = uniformBuffers[i].value,
                                    .offset = 0,
                                    .range = sizeof(UniformBufferObject)
                                }
                            },
                            .texelBufferView = {}
                        },
                        WriteDescriptorSet {
                            .dstBinding = 1,
                            .dstArrayElement = 0,
                            .descriptorCount = 1,
                            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                            .imageInfo = {
                                VkDescriptorImageInfo {
                                    .sampler = sampler,
                                    .imageView = imageView,
                                    .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                                }
                            },
                            .bufferInfo = {},
                            .texelBufferView = {}
                        }
                    }
                );
              }
              transferCommandBuffer = createCommandBuffer(device, transferCommandPool);
              graphicsCommandBuffers = createCommandBuffers(device, graphicsCommandPool, MAX_FRAMES_IN_FLIGHT);

              transitionImageLayout(device, transferQueue.value, transferCommandPool, image.value, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
              copyBufferToImage(device, transferQueue.value, transferCommandPool, imageStaging.value, image.value, static_cast<uint32_t>(image.width), static_cast<uint32_t>(image.height));
              transitionImageLayout(device, transferQueue.value, transferCommandPool, image.value, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

              copyBuffer(
                  device,
                  transferQueue.value,
                  transferCommandPool,
                  vertexStagingBuffer.memorySize,
                  vertexStagingBuffer.value,
                  vertexBuffer.value
              );

              copyBuffer(
                  device,
                  transferQueue.value,
                  transferCommandPool,
                  indexStagingBuffer.memorySize,
                  indexStagingBuffer.value,
                  indexBuffer.value
              );

              imageAvailableSemaphores = createSemaphores(device, MAX_FRAMES_IN_FLIGHT);
              renderFinishedSemaphores = createSemaphores(device, MAX_FRAMES_IN_FLIGHT);
              inFlightFences = createFences(device, MAX_FRAMES_IN_FLIGHT);
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
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

              updateUniformBuffer(currentFrame);

              vkResetFences(device, 1, &inFlightFences[currentFrame]);

              vkResetCommandBuffer(graphicsCommandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
              recordCommandBuffer(
                  graphicsCommandBuffers[currentFrame],
                  imageIndex,
                  renderPass,
                  swapChainFrameBuffers,
                  swapChain.extent,
                  graphicsPipeline.value,
                  vertexBuffer.value,
                  indexBuffer.value,
                  graphicsPipeline.layout,
                  descriptorSets,
                  currentFrame
              );

              VkSubmitInfo submitInfo{};
              submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

              VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
              VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
              submitInfo.waitSemaphoreCount = 1;
              submitInfo.pWaitSemaphores = waitSemaphores;
              submitInfo.pWaitDstStageMask = waitStages;

              submitInfo.commandBufferCount = 1;
              submitInfo.pCommandBuffers = &graphicsCommandBuffers[currentFrame];

              VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
              submitInfo.signalSemaphoreCount = 1;
              submitInfo.pSignalSemaphores = signalSemaphores;

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
              destroySemaphores(renderFinishedSemaphores, device);
              destroySemaphores(imageAvailableSemaphores, device);
              destroyFences(inFlightFences, device);

              destroyCommandBuffers(graphicsCommandBuffers, graphicsCommandPool, device);
              destroyCommandBuffer(transferCommandBuffer, transferCommandPool, device);
              destroyDescriptorSets(descriptorSets);
              destroyDescriptorPool(descriptorPool, device);
              destroySampler(sampler, device);
              destroyBuffers(uniformBuffers, device);
              destroyBuffer(indexBuffer, device);
              destroyBuffer(indexStagingBuffer, device);
              destroyBuffer(vertexBuffer, device);
              destroyBuffer(vertexStagingBuffer, device);
              destroyImageView(imageView, device);
              destroyImage(image, device);
              destroyBuffer(imageStaging, device);
              destroyFrameBuffers(swapChainFrameBuffers, device);
              destroyPipeline(graphicsPipeline, device);
              destroyDescriptorSetLayout(descriptorSetLayout, device);
              destroyRenderPass(renderPass, device);
              destroyImageView(depthImageView, device);
              destroyImage(depthImage, device);
              destroyImageViews(swapChainImageViews, device);
              destroySwapChain(swapChain, device);
              destroyCommandPool(graphicsCommandPool, device);
              destroyCommandPool(transferCommandPool, device);
              destroyQueue(presentQueue);
              destroyQueue(graphicsQueue);
              destroyQueue(transferQueue);
              destroyDevice(device);
              destroyPhysicalDevice(physicalDevice);
              destroySurface(surface, instance);
              destroyDebugUtilsMessenger(debugUtilsMessenger, instance);
              destroyInstance(instance);
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

        private:

          VkCommandBuffer beginSingleTimeCommands(VkDevice& device, VkCommandPool& commandPool) {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = commandPool;
            allocInfo.commandBufferCount = 1;

            VkCommandBuffer commandBuffer;
            vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
              throw std::runtime_error(CALL_INFO() + ": failed to begin command buffer!");
            }

            return commandBuffer;
          }

          void endSingleTimeCommands(VkDevice& device, VkQueue& queue, VkCommandPool& commandPool, VkCommandBuffer commandBuffer) {
            if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
              throw std::runtime_error(CALL_INFO() + ": failed to end command buffer!");
            }

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(queue);

            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
          }

          void copyBufferToImage(VkDevice& device, VkQueue& queue, VkCommandPool& commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
            VkCommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

            VkBufferImageCopy region{};
            region.bufferOffset = 0;
            region.bufferRowLength = 0;
            region.bufferImageHeight = 0;
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = 0;
            region.imageSubresource.baseArrayLayer = 0;
            region.imageSubresource.layerCount = 1;
            region.imageOffset = {0, 0, 0};
            region.imageExtent = {
                width,
                height,
                1
            };

            vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            endSingleTimeCommands(device, queue, commandPool, commandBuffer);
          }

          void copyBuffer(
              VkDevice& device,
              VkQueue& queue,
              VkCommandPool& commandPool,
              VkDeviceSize size,
              VkBuffer& srcBuffer,
              VkBuffer& dstBuffer
          ) {
            VkCommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

            VkBufferCopy copyRegion{};
            copyRegion.size = size;
            vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

            endSingleTimeCommands(device, queue, commandPool, commandBuffer);
          }

          void transitionImageLayout(VkDevice& device, VkQueue& queue, VkCommandPool& commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
            VkCommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = oldLayout;
            barrier.newLayout = newLayout;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = image;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;

            VkPipelineStageFlags sourceStage;
            VkPipelineStageFlags destinationStage;

            if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
              barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

              if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT) {
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
              }
            } else {
              barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            }

            if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
              barrier.srcAccessMask = 0;
              barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

              sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
              destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
              barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
              barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

              sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
              destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            } else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
              barrier.srcAccessMask = 0;
              barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

              sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
              destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            } else {
              throw std::invalid_argument(CALL_INFO() + ": unsupported layout transition!");
            }

            vkCmdPipelineBarrier(
                commandBuffer,
                sourceStage, destinationStage,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
            );

            endSingleTimeCommands(device, queue, commandPool, commandBuffer);
          }

          void recordCommandBuffer(
              VkCommandBuffer& commandBuffer,
              uint32_t imageIndex,
              VkRenderPass& renderPass,
              std::vector<VkFramebuffer>& swapChainFramebuffers,
              VkExtent2D& swapChainExtent,
              VkPipeline& graphicsPipeline,
              VkBuffer& vertexBuffer,
              VkBuffer& indexBuffer,
              VkPipelineLayout& pipelineLayout,
              std::vector<VkDescriptorSet>& descriptorSets,
              std::size_t currentFrame
          ) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
              throw std::runtime_error(CALL_INFO() + ": failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass;
            renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapChainExtent;

            std::vector<VkClearValue> clearValues = {
                VkClearValue {
                    .color = {0.0f, 0.0f, 0.0f, 1.0f}
                },
                VkClearValue {
                    .depthStencil = {1.0f, 0}
                }
            };

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

            VkBuffer vertexBuffers[] = {vertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

            vkCmdEndRenderPass(commandBuffer);

            if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
              throw std::runtime_error(CALL_INFO() + ": failed to record command buffer!");
            }
          }

          void reCreateSwapChain(int width, int height) {
            std::cout << __FUNCTION__ << " width: " << width << " height: " << height << std::endl;

            vkDeviceWaitIdle(device);

            destroyFrameBuffers(swapChainFrameBuffers, device);
            destroyPipeline(graphicsPipeline, device);
            destroyRenderPass(renderPass, device);
            destroyImageView(depthImageView, device);
            destroyImage(depthImage, device);
            destroyImageViews(swapChainImageViews, device);
            destroySwapChain(swapChain, device);

            physicalDevice.swapChainSupportDetails = querySwapChainSupport(physicalDevice.value, surface);

            swapChain = createSwapChain(physicalDevice.swapChainSupportDetails.value(), physicalDevice.queueFamilyIndexInfo, surface, device, width, height);
            swapChainImages = createSwapChainImages(device, swapChain.value);
            swapChainImageViews = createImageViews(device, swapChainImages, swapChain.format);

            depthImage = createImage(
                physicalDevice.value,
                device,
                swapChain.width,
                swapChain.height,
                findDepthFormat(physicalDevice.value),
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );
            depthImageView = createImageView(device, depthImage.value, depthImage.format, VK_IMAGE_ASPECT_DEPTH_BIT);
            transitionImageLayout(device, transferQueue.value, transferCommandPool, depthImage.value, depthImage.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

            renderPass = createRenderPass(
                device,
                RenderPassCreateInfo {
                    .attachments = {
                        VkAttachmentDescription {
                            .format = swapChain.format,
                            .samples = VK_SAMPLE_COUNT_1_BIT,
                            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
                        },
                        VkAttachmentDescription {
                            .format = depthImage.format,
                            .samples = VK_SAMPLE_COUNT_1_BIT,
                            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                            .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                        }
                    },
                    .subPasses = {
                        SubPassDescription {
                            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                            .colorAttachments = {
                                VkAttachmentReference {
                                    .attachment = 0,
                                    .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                                }
                            },
                            .depthStencilAttachment = VkAttachmentReference {
                                .attachment = 1,
                                .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                            }
                        }
                    },
                    .dependencies = {
                        VkSubpassDependency {
                            .srcSubpass = VK_SUBPASS_EXTERNAL,
                            .dstSubpass = 0,
                            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                            .srcAccessMask = 0,
                            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
                        }
                    }
                }
            );
            graphicsPipeline = createGraphicsPipeline(
                device,
                swapChain.extent,
                {"resources/shader/shader-4.vert.spv", "resources/shader/shader-4.frag.spv"},
                renderPass,
                VK_FRONT_FACE_COUNTER_CLOCKWISE,
                {descriptorSetLayout},
                {Vertex::getBindingDescription()},
                Vertex::getAttributeDescriptions()
            );

            std::vector<FrameBufferCreateInfo> frameBufferCreateInfoVector;
            frameBufferCreateInfoVector.resize(swapChainImageViews.size());
            for (std::size_t i = 0; i < frameBufferCreateInfoVector.size(); i++) {
              frameBufferCreateInfoVector[i] = FrameBufferCreateInfo {
                  .flags = 0,
                  .renderPass = renderPass,
                  .attachments = {
                      swapChainImageViews[i],
                      depthImageView
                  },
                  .width = swapChain.extent.width,
                  .height = swapChain.extent.height,
                  .layers = 1
              };
            }
            swapChainFrameBuffers = createFrameBuffers(device, frameBufferCreateInfoVector);
          }

          void updateUniformBuffer(uint32_t currentImage) {
            static auto startTime = std::chrono::high_resolution_clock::now();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

            UniformBufferObject ubo = {};
            ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ubo.proj = glm::perspective(glm::radians(45.0f), (float) swapChain.extent.width / (float) swapChain.extent.height, 0.1f, 10.0f);
            ubo.proj[1][1] *= -1;

            void* data;
            vkMapMemory(device, uniformBuffers[currentImage].memory, 0, sizeof(ubo), 0, &data);
            std::memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, uniformBuffers[currentImage].memory);
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

  TEST_F(UiTestsD, test1) {
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
