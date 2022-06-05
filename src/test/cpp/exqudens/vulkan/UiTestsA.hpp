#pragma once

#include <cstddef>
#include <cstring>
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

        private:
          inline static const size_t MAX_FRAMES_IN_FLIGHT = 2;

          std::vector<Vertex> vertexVector = {};
          std::vector<uint16_t> indexVector = {};

          Environment environment = {};
          Context context = {};
          Instance instance = {};
          Messenger messenger = {};
          DebugUtilsMessenger debugUtilsMessenger = {};
          Surface surface = {};
          PhysicalDevice physicalDevice = {};
          Device device = {};
          SwapChain swapChain = {};
          Image depthImage = {};
          ImageView depthImageView = {};
          Buffer textureBuffer = {};
          Image textureImage = {};
          ImageView textureImageView = {};
          Buffer vertexStagingBuffer = {};
          Buffer vertexBuffer = {};
          Buffer indexStagingBuffer = {};
          Buffer indexBuffer = {};
          std::vector<Buffer> uniformBuffers = std::vector<Buffer>(MAX_FRAMES_IN_FLIGHT);
          Sampler sampler = {};
          std::vector<Semaphore> imageAvailableSemaphores = std::vector<Semaphore>(MAX_FRAMES_IN_FLIGHT);
          std::vector<Semaphore> renderFinishedSemaphores = std::vector<Semaphore>(MAX_FRAMES_IN_FLIGHT);
          std::vector<Fence> inFlightFences = std::vector<Fence>(MAX_FRAMES_IN_FLIGHT);
          RenderPass renderPass = {};
          DescriptorSetLayout descriptorSetLayout = {};
          Pipeline pipeline = {};
          DescriptorPool descriptorPool = {};
          std::vector<DescriptorSet> descriptorSets = std::vector<DescriptorSet>(MAX_FRAMES_IN_FLIGHT);
          //Queue transferQueue = {};
          //Queue graphicsQueue = {};
          //Queue presentQueue = {};
          //std::vector<ImageView> swapChainImageViews = {};

        public:

          void create(
              const std::vector<std::string>& arguments,
              const std::vector<const char*>& glfwInstanceRequiredExtensions,
              GLFWwindow* window,
              uint32_t width,
              uint32_t height
          ) {
            try {
              vertexVector = {
                  {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                  {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                  {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                  {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

                  {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                  {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                  {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                  {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
              };

              indexVector = {
                  0, 1, 2, 2, 3, 0,
                  4, 5, 6, 6, 7, 4
              };

              environment = Environment();

              context = environment.createContext(
                  ContextCreateInfo()
                      .setEnvironmentVariables({{"VK_LAYER_PATH", arguments.front().c_str()}})
              );

              std::vector<const char*> enabledExtensionNames = glfwInstanceRequiredExtensions;
              enabledExtensionNames.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

              instance = environment.createInstance(
                  context,
                  InstanceCreateInfo()
                      .setEnabledLayerNames({"VK_LAYER_KHRONOS_validation"})
                      .setEnabledExtensionNames(enabledExtensionNames)
                      .setApplicationInfo(
                          vk::ApplicationInfo()
                              .setPApplicationName("Exqudens Application")
                              .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
                              .setPEngineName("Exqudens Engine")
                              .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
                              .setApiVersion(VK_API_VERSION_1_0)
                      )
              );

              messenger = environment.createMessenger(
                  std::cout,
                  vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                  vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose,
                  {}
              );

              debugUtilsMessenger = environment.createDebugUtilsMessenger(
                  instance,
                  messenger,
                  vk::DebugUtilsMessengerCreateInfoEXT()
                      .setPUserData(&messenger)
                      .setPfnUserCallback(&Messenger::callback)
                      .setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
                      .setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
              );

              VkSurfaceKHR vkSurface = nullptr;
              auto vkInstance = static_cast<VkInstance>(*(*instance.value));
              if (glfwCreateWindowSurface(vkInstance, window, nullptr, &vkSurface) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to create surface!");
              }
              if (vkSurface == nullptr) {
                throw std::runtime_error(CALL_INFO() + ": surface is null!");
              }
              surface = environment.createSurface(instance, vkSurface);

              physicalDevice = environment.createPhysicalDevice(
                  instance,
                  PhysicalDeviceCreateInfo()
                      .setEnabledExtensionNames({VK_KHR_SWAPCHAIN_EXTENSION_NAME})
                      .setFeatures(vk::PhysicalDeviceFeatures().setSamplerAnisotropy(true))
                      .setQueueTypes({vk::QueueFlagBits::eCompute, vk::QueueFlagBits::eTransfer, vk::QueueFlagBits::eGraphics})
                      .setSurface(*surface.reference())
                      .setQueuePriorities(1.0f)
              );

              device = environment.createDevice(
                  physicalDevice,
                  vk::DeviceCreateInfo()
                      .setQueueCreateInfos(physicalDevice.uniqueQueueCreateInfos)
                      .setPEnabledFeatures(&physicalDevice.createInfo.features)
                      .setPEnabledExtensionNames(physicalDevice.createInfo.enabledExtensionNames)
                      .setPEnabledLayerNames(instance.createInfo.enabledLayerNames)
              );

              swapChain = environment.createSwapChain(
                  device,
                  environment.swapChainCreateInfo(physicalDevice, surface, width, height)
                      .setImageSharingMode(
                          physicalDevice.graphicsQueueCreateInfos.front() == physicalDevice.presentQueueCreateInfos.front()
                          ? vk::SharingMode::eExclusive
                          : vk::SharingMode::eConcurrent
                      )
              );

              depthImage = environment.createImage(
                  physicalDevice,
                  device,
                  vk::ImageCreateInfo()
                      .setImageType(vk::ImageType::e2D)
                      .setFormat(environment.imageDepthFormat(physicalDevice))
                      .setExtent(
                          vk::Extent3D()
                              .setWidth(swapChain.createInfo.imageExtent.width)
                              .setHeight(swapChain.createInfo.imageExtent.height)
                              .setDepth(1)
                      )
                      .setMipLevels(1)
                      .setArrayLayers(1)
                      .setSamples(vk::SampleCountFlagBits::e1)
                      .setTiling(vk::ImageTiling::eOptimal)
                      .setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment)
                      .setSharingMode(vk::SharingMode::eExclusive)
                      .setQueueFamilyIndices({})
                      .setInitialLayout(vk::ImageLayout::eUndefined),
                  vk::MemoryPropertyFlagBits::eDeviceLocal
              );

              depthImageView = environment.createImageView(
                  device,
                  vk::ImageViewCreateInfo()
                      .setImage(*depthImage.reference())
                      .setFormat(depthImage.createInfo.format)
                      .setViewType(vk::ImageViewType::e2D)
                      .setFlags({})
                      .setComponents({})
                      .setSubresourceRange(
                          vk::ImageSubresourceRange()
                              .setAspectMask(vk::ImageAspectFlagBits::eDepth)
                              .setBaseMipLevel(0)
                              .setLevelCount(1)
                              .setBaseArrayLayer(0)
                              .setLayerCount(1)
                      )
              );

              unsigned int tmpImageWidth, tmpImageHeight, tmpImageDepth;
              std::vector<unsigned char> tmpImageData;
              TestUtils::readPng(
                  std::filesystem::path().append("resources").append("png").append("texture.png").make_preferred().string(),
                  tmpImageWidth,
                  tmpImageHeight,
                  tmpImageDepth,
                  tmpImageData
              );

              textureBuffer = environment.createBuffer(
                  physicalDevice,
                  device,
                  vk::BufferCreateInfo()
                      .setSize(tmpImageWidth * tmpImageHeight * tmpImageDepth)
                      .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
                      .setSharingMode(vk::SharingMode::eExclusive),
                  vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
              );
              void* tmpData = textureBuffer.memoryReference().mapMemory(0, textureBuffer.size);
              std::memcpy(tmpData, tmpImageData.data(), static_cast<size_t>(textureBuffer.size));
              textureBuffer.memoryReference().unmapMemory();
              textureImage = environment.createImage(
                  physicalDevice,
                  device,
                  vk::ImageCreateInfo()
                      .setImageType(vk::ImageType::e2D)
                      .setFormat(vk::Format::eR8G8B8A8Srgb)
                      .setExtent(
                          vk::Extent3D()
                              .setWidth(tmpImageWidth)
                              .setHeight(tmpImageHeight)
                              .setDepth(1)
                      )
                      .setMipLevels(1)
                      .setArrayLayers(1)
                      .setSamples(vk::SampleCountFlagBits::e1)
                      .setTiling(vk::ImageTiling::eOptimal)
                      .setUsage(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled)
                      .setSharingMode(vk::SharingMode::eExclusive)
                      .setQueueFamilyIndices({})
                      .setInitialLayout(vk::ImageLayout::eUndefined),
                  vk::MemoryPropertyFlagBits::eDeviceLocal
              );
              textureImageView = environment.createImageView(
                  device,
                  vk::ImageViewCreateInfo()
                      .setImage(*textureImage.reference())
                      .setFormat(textureImage.createInfo.format)
                      .setViewType(vk::ImageViewType::e2D)
                      .setFlags({})
                      .setComponents({})
                      .setSubresourceRange(
                          vk::ImageSubresourceRange()
                              .setAspectMask(vk::ImageAspectFlagBits::eColor)
                              .setBaseMipLevel(0)
                              .setLevelCount(1)
                              .setBaseArrayLayer(0)
                              .setLayerCount(1)
                      )
              );

              vertexStagingBuffer = environment.createBuffer(
                  physicalDevice,
                  device,
                  vk::BufferCreateInfo()
                      .setSize(sizeof(vertexVector[0]) * vertexVector.size())
                      .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
                      .setSharingMode(vk::SharingMode::eExclusive),
                  vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
              );
              tmpData = vertexStagingBuffer.memoryReference().mapMemory(0, vertexStagingBuffer.size);
              std::memcpy(tmpData, vertexVector.data(), static_cast<size_t>(vertexStagingBuffer.size));
              vertexStagingBuffer.memoryReference().unmapMemory();
              vertexBuffer = environment.createBuffer(
                  physicalDevice,
                  device,
                  vk::BufferCreateInfo()
                      .setSize(vertexStagingBuffer.size)
                      .setUsage(vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer)
                      .setSharingMode(vk::SharingMode::eExclusive),
                  vk::MemoryPropertyFlagBits::eDeviceLocal
              );

              indexStagingBuffer = environment.createBuffer(
                  physicalDevice,
                  device,
                  vk::BufferCreateInfo()
                      .setSize(sizeof(indexVector[0]) * indexVector.size())
                      .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
                      .setSharingMode(vk::SharingMode::eExclusive),
                  vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
              );
              tmpData = indexStagingBuffer.memoryReference().mapMemory(0, indexStagingBuffer.size);
              std::memcpy(tmpData, indexVector.data(), static_cast<size_t>(indexStagingBuffer.size));
              indexStagingBuffer.memoryReference().unmapMemory();
              indexBuffer = environment.createBuffer(
                  physicalDevice,
                  device,
                  vk::BufferCreateInfo()
                      .setSize(indexStagingBuffer.size)
                      .setUsage(vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer)
                      .setSharingMode(vk::SharingMode::eExclusive),
                  vk::MemoryPropertyFlagBits::eDeviceLocal
              );

              std::generate_n(uniformBuffers.begin(), uniformBuffers.size(), [this]() {
                return environment.createBuffer(
                    physicalDevice,
                    device,
                    vk::BufferCreateInfo()
                        .setSize(sizeof(UniformBufferObject) * uniformBuffers.size())
                        .setUsage(vk::BufferUsageFlagBits::eUniformBuffer)
                        .setSharingMode(vk::SharingMode::eExclusive),
                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
                );
              });

              sampler = environment.createSampler(
                  device,
                  vk::SamplerCreateInfo()
                      .setMagFilter(vk::Filter::eLinear)
                      .setMinFilter(vk::Filter::eLinear)
                      .setMipmapMode(vk::SamplerMipmapMode::eLinear)
                      .setAddressModeU(vk::SamplerAddressMode::eRepeat)
                      .setAddressModeV(vk::SamplerAddressMode::eRepeat)
                      .setAddressModeW(vk::SamplerAddressMode::eRepeat)
                      .setCompareOp(vk::CompareOp::eAlways)
                      .setBorderColor(vk::BorderColor::eIntOpaqueBlack)
                      .setUnnormalizedCoordinates(false)
                      .setCompareEnable(false)
                      .setAnisotropyEnable(physicalDevice.createInfo.features.samplerAnisotropy)
                      .setMaxAnisotropy(physicalDevice.createInfo.features.samplerAnisotropy ? physicalDevice.reference().getProperties().limits.maxSamplerAnisotropy : 0)
              );

              std::generate_n(imageAvailableSemaphores.begin(), imageAvailableSemaphores.size(), [this]() {
                return environment.createSemaphore(
                    device,
                    vk::SemaphoreCreateInfo()
                );
              });

              std::generate_n(renderFinishedSemaphores.begin(), renderFinishedSemaphores.size(), [this]() {
                return environment.createSemaphore(
                    device,
                    vk::SemaphoreCreateInfo()
                );
              });

              std::generate_n(inFlightFences.begin(), inFlightFences.size(), [this]() {
                return environment.createFence(
                    device,
                    vk::FenceCreateInfo()
                );
              });

              renderPass = environment.createRenderPass(
                  device,
                  RenderPassCreateInfo()
                      .setAttachments({
                          vk::AttachmentDescription()
                              .setFormat(swapChain.createInfo.imageFormat)
                              .setSamples(vk::SampleCountFlagBits::e1)
                              .setLoadOp(vk::AttachmentLoadOp::eClear)
                              .setStencilLoadOp(vk::AttachmentLoadOp::eClear)
                              .setStoreOp(vk::AttachmentStoreOp::eDontCare)
                              .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                              .setInitialLayout(vk::ImageLayout::eUndefined)
                              .setFinalLayout(vk::ImageLayout::ePresentSrcKHR),
                          vk::AttachmentDescription()
                              .setFormat(depthImage.createInfo.format)
                              .setSamples(vk::SampleCountFlagBits::e1)
                              .setLoadOp(vk::AttachmentLoadOp::eClear)
                              .setStencilLoadOp(vk::AttachmentLoadOp::eClear)
                              .setStoreOp(vk::AttachmentStoreOp::eDontCare)
                              .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                              .setInitialLayout(vk::ImageLayout::eUndefined)
                              .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
                      })
                      .setSubpasses({
                          SubpassDescription()
                              .setColorAttachments({
                                  vk::AttachmentReference()
                                      .setAttachment(0)
                                      .setLayout(vk::ImageLayout::eColorAttachmentOptimal)
                              })
                              .setDepthStencilAttachment(
                                  vk::AttachmentReference()
                                      .setAttachment(1)
                                      .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
                              )
                              .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                      })
                      .setDependencies({
                          vk::SubpassDependency()
                              .setSrcSubpass(VK_SUBPASS_EXTERNAL)
                              .setDstSubpass(0)
                              .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
                              .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
                              .setSrcAccessMask(vk::AccessFlagBits::eNoneKHR)
                              .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
                      })
              );

              descriptorSetLayout = environment.createDescriptorSetLayout(
                  device,
                  DescriptorSetLayoutCreateInfo()
                      .setBindings({
                          vk::DescriptorSetLayoutBinding()
                              .setBinding(0)
                              .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                              .setDescriptorCount(1)
                              .setStageFlags(vk::ShaderStageFlagBits::eVertex),
                          vk::DescriptorSetLayoutBinding()
                              .setBinding(1)
                              .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                              .setDescriptorCount(1)
                              .setStageFlags(vk::ShaderStageFlagBits::eFragment)
                      })
              );

              pipeline = environment.createPipeline(
                  device,
                  vk::PipelineCacheCreateInfo(),
                  PipelineLayoutCreateInfo()
                      .setSetLayouts({*descriptorSetLayout.reference()}),
                  {"resources/shader/shader-4.vert.spv", "resources/shader/shader-4.frag.spv"},
                  GraphicsPipelineCreateInfo()
                      .setRenderPass(*renderPass.reference())
                      .setSubpass(0)
                      .setVertexInputState(
                          PipelineVertexInputStateCreateInfo()
                              .setVertexBindingDescriptions({Vertex::getBindingDescription()})
                              .setVertexAttributeDescriptions(Vertex::getAttributeDescriptions())
                      )
                      .setInputAssemblyState(
                          vk::PipelineInputAssemblyStateCreateInfo()
                              .setTopology(vk::PrimitiveTopology::eTriangleList)
                              .setPrimitiveRestartEnable(false)
                      )
                      .setViewportState(
                          PipelineViewportStateCreateInfo()
                              .setViewports({
                                  vk::Viewport()
                                      .setWidth((float) swapChain.createInfo.imageExtent.width)
                                      .setHeight((float) swapChain.createInfo.imageExtent.height)
                                      .setMinDepth(0.0)
                                      .setMaxDepth(1.0)
                                      .setX(0.0)
                                      .setY(0.0)
                              })
                              .setScissors({
                                  vk::Rect2D()
                                      .setOffset({0, 0})
                                      .setExtent(swapChain.createInfo.imageExtent)
                              })
                      )
                      .setRasterizationState(
                          vk::PipelineRasterizationStateCreateInfo()
                              .setDepthClampEnable(false)
                              .setRasterizerDiscardEnable(false)
                              .setPolygonMode(vk::PolygonMode::eFill)
                              .setCullMode(vk::CullModeFlagBits::eBack)
                              .setFrontFace(vk::FrontFace::eCounterClockwise)
                              .setLineWidth(1.0)
                              .setDepthBiasEnable(false)
                              .setDepthBiasConstantFactor(0.0)
                              .setDepthBiasClamp(0.0)
                              .setDepthBiasSlopeFactor(0.0)
                      )
                      .setMultisampleState(
                          vk::PipelineMultisampleStateCreateInfo()
                              .setRasterizationSamples(vk::SampleCountFlagBits::e1)
                              .setSampleShadingEnable(false)
                              .setMinSampleShading(1.0)
                              .setPSampleMask(nullptr)
                              .setAlphaToCoverageEnable(false)
                              .setAlphaToOneEnable(false)
                      )
                      .setDepthStencilState(
                          vk::PipelineDepthStencilStateCreateInfo()
                              .setDepthTestEnable(true)
                              .setDepthWriteEnable(true)
                              .setDepthCompareOp(vk::CompareOp::eLess)
                              .setDepthBoundsTestEnable(false)
                              .setStencilTestEnable(false)
                              .setFront({})
                              .setBack({})
                              .setMinDepthBounds(0.0)
                              .setMaxDepthBounds(1.0)
                      )
                      .setColorBlendState(
                          PipelineColorBlendStateCreateInfo()
                              .setLogicOpEnable(false)
                              .setLogicOp(vk::LogicOp::eCopy)
                              .setBlendConstants({0.0f, 0.0f, 0.0f, 0.0f})
                              .setAttachments({
                                  vk::PipelineColorBlendAttachmentState()
                                      .setBlendEnable(false)
                                      .setColorBlendOp(vk::BlendOp::eAdd)
                                      .setSrcColorBlendFactor(vk::BlendFactor::eOne)
                                      .setDstColorBlendFactor(vk::BlendFactor::eZero)
                                      .setAlphaBlendOp(vk::BlendOp::eAdd)
                                      .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
                                      .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
                                      .setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
                              })
                      )
              );

              descriptorPool = environment.createDescriptorPool(
                  device,
                  DescriptorPoolCreateInfo()
                      .setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
                      .setMaxSets(MAX_FRAMES_IN_FLIGHT)
                      .setPoolSizes({
                          vk::DescriptorPoolSize()
                              .setType(vk::DescriptorType::eUniformBuffer)
                              .setDescriptorCount(MAX_FRAMES_IN_FLIGHT),
                          vk::DescriptorPoolSize()
                              .setType(vk::DescriptorType::eCombinedImageSampler)
                              .setDescriptorCount(MAX_FRAMES_IN_FLIGHT)
                      })
              );

              for (auto& descriptorSet : descriptorSets) {
                descriptorSet = environment.createDescriptorSet(
                    device,
                    DescriptorSetAllocateInfo()
                        .setDescriptorPool(*descriptorPool.reference())
                        .setSetLayouts({*descriptorSetLayout.reference()})
                        .setDescriptorSetCount(1)
                );
              }

              for (size_t i = 0; i < descriptorSets.size(); i++) {
                std::vector<vk::DescriptorBufferInfo> writeBufferInfo = {
                    vk::DescriptorBufferInfo()
                        .setBuffer(*uniformBuffers[i].reference())
                        .setOffset(0)
                        .setRange(sizeof(UniformBufferObject))
                };
                vk::WriteDescriptorSet writeBuffer = vk::WriteDescriptorSet()
                    .setDstSet(*descriptorSets[i].reference())
                    .setDstBinding(0)
                    .setDstArrayElement(0)
                    .setDescriptorCount(1)
                    .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                    .setBufferInfo(writeBufferInfo);

                std::vector<vk::DescriptorImageInfo> writeImageInfo = {
                    vk::DescriptorImageInfo()
                        .setSampler(*sampler.reference())
                        .setImageView(*textureImageView.reference())
                        .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                };
                vk::WriteDescriptorSet writeImage = vk::WriteDescriptorSet()
                    .setDstSet(*descriptorSets[i].reference())
                    .setDstBinding(1)
                    .setDstArrayElement(0)
                    .setDescriptorCount(1)
                    .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                    .setImageInfo(writeImageInfo);

                device.reference().updateDescriptorSets({writeBuffer, writeImage}, {});
              }

              /*transferQueue = environment.createQueue(
                  device,
                  physicalDevice.transferQueueCreateInfos.front().queueFamilyIndex,
                  0
              );
              graphicsQueue = environment.createQueue(
                  device,
                  physicalDevice.graphicsQueueCreateInfos.front().queueFamilyIndex,
                  0
              );
              presentQueue = environment.createQueue(
                  device,
                  physicalDevice.presentQueueCreateInfos.front().queueFamilyIndex,
                  0
              );*/

              /*swapChainImageViews = environment.createSwapChainImageViews(
                  device,
                  swapChain
              );*/

              std::cout << std::format("context.createInfo.environmentVariables['VK_LAYER_PATH']: '{}'", context.createInfo.environmentVariables["VK_LAYER_PATH"]) << std::endl;
              std::cout << std::format("context.id: '{}'", context.id) << std::endl;
              std::cout << std::format("instance.id: '{}'", instance.id) << std::endl;
              std::cout << std::format("messenger.id: '{}'", messenger.id) << std::endl;
              std::cout << std::format("debugUtilsMessenger.id: '{}'", debugUtilsMessenger.id) << std::endl;
              std::cout << std::format("surface.id: '{}'", surface.id) << std::endl;
              std::cout << std::format("physicalDevice.id: '{}'", physicalDevice.id) << std::endl;
              std::cout << std::format("device.id: '{}'", device.id) << std::endl;
              std::cout << std::format("swapChain.id: '{}'", swapChain.id) << std::endl;
              std::cout << std::format("depthImage.id: '{}'", depthImage.id) << std::endl;
              std::cout << std::format("depthImageView.id: '{}'", depthImageView.id) << std::endl;
              std::cout << std::format("textureBuffer.id: '{}'", textureBuffer.id) << std::endl;
              std::cout << std::format("textureImage.id: '{}'", textureImage.id) << std::endl;
              std::cout << std::format("textureImageView.id: '{}'", textureImageView.id) << std::endl;
              std::cout << std::format("vertexStagingBuffer.id: '{}'", vertexStagingBuffer.id) << std::endl;
              std::cout << std::format("vertexBuffer.id: '{}'", vertexBuffer.id) << std::endl;
              std::cout << std::format("indexStagingBuffer.id: '{}'", indexStagingBuffer.id) << std::endl;
              std::cout << std::format("indexBuffer.id: '{}'", indexBuffer.id) << std::endl;
              std::ranges::for_each(uniformBuffers, [](auto& o1) {std::cout << std::format("uniformBuffer.id: '{}'", o1.id) << std::endl;});
              std::cout << std::format("sampler.id: '{}'", sampler.id) << std::endl;
              std::ranges::for_each(imageAvailableSemaphores, [](auto& o1) {std::cout << std::format("imageAvailableSemaphore.id: '{}'", o1.id) << std::endl;});
              std::ranges::for_each(renderFinishedSemaphores, [](auto& o1) {std::cout << std::format("renderFinishedSemaphore.id: '{}'", o1.id) << std::endl;});
              std::ranges::for_each(inFlightFences, [](auto& o1) {std::cout << std::format("inFlightFence.id: '{}'", o1.id) << std::endl;});
              std::cout << std::format("renderPass.id: '{}'", renderPass.id) << std::endl;
              std::cout << std::format("descriptorSetLayout.id: '{}'", descriptorSetLayout.id) << std::endl;
              std::cout << std::format("pipeline.id: '{}'", pipeline.id) << std::endl;
              std::cout << std::format("descriptorPool.id: '{}'", descriptorPool.id) << std::endl;
              std::ranges::for_each(descriptorSets, [](auto& o1) {std::cout << std::format("descriptorSet.id: '{}'", o1.id) << std::endl;});
              //std::cout << std::format("transferQueue.id: '{}'", transferQueue.id) << std::endl;
              //std::cout << std::format("graphicsQueue.id: '{}'", graphicsQueue.id) << std::endl;
              //std::cout << std::format("presentQueue.id: '{}'", presentQueue.id) << std::endl;
              //std::ranges::for_each(swapChainImageViews, [](const auto& swapChainImageView) {std::cout << std::format("swapChainImageView.id: '{}'", swapChainImageView.id) << std::endl;});
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

              //glfwSetWindowUserPointer(window, this);
              //glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

              uint32_t glfwExtensionCount = 0;
              const char** glfwExtensions;
              glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
              std::vector<const char*> glfwInstanceRequiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

              renderer = new Renderer();
              renderer->create(
                  arguments,
                  glfwInstanceRequiredExtensions,
                  window,
                  static_cast<uint32_t>(width),
                  static_cast<uint32_t>(height)
              );

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

          /*static void frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
            try {
              auto* app = reinterpret_cast<TestUiApplication*>(glfwGetWindowUserPointer(window));
              app->renderer->resized = true;
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }*/

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
