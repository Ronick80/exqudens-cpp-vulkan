#pragma once

#include <string>
#include <optional>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <ostream>
#include <cstdint>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Export.hpp"
#include "exqudens/vulkan/model/Functions.hpp"
#include "exqudens/vulkan/Logger.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"
#include "exqudens/vulkan/model/QueueFamilyIndexInfo.hpp"
#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"
#include "exqudens/vulkan/model/PhysicalDevice.hpp"
#include "exqudens/vulkan/model/Queue.hpp"
#include "exqudens/vulkan/model/SwapChain.hpp"
#include "exqudens/vulkan/model/BufferCreateInfo.hpp"
#include "exqudens/vulkan/model/Buffer.hpp"
#include "exqudens/vulkan/model/ImageCreateInfo.hpp"
#include "exqudens/vulkan/model/Image.hpp"
#include "exqudens/vulkan/model/FrameBufferCreateInfo.hpp"
#include "exqudens/vulkan/model/RenderPassCreateInfo.hpp"
#include "exqudens/vulkan/model/DescriptorSetLayoutCreateInfo.hpp"
#include "exqudens/vulkan/model/DescriptorPoolCreateInfo.hpp"
#include "exqudens/vulkan/model/WriteDescriptorSet.hpp"
#include "exqudens/vulkan/model/Shader.hpp"
#include "exqudens/vulkan/model/PipelineLayoutCreateInfo.hpp"
#include "exqudens/vulkan/model/GraphicsPipelineCreateInfo.hpp"
#include "exqudens/vulkan/model/Pipeline.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Factory {

    public:

      Functions functions;

      // utility

      virtual void setEnvironmentVariable(const std::string& name, const std::string& value);

      virtual std::optional<std::string> getEnvironmentVariable(const std::string& name);

      virtual std::function<std::string(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          const std::string&
      )> createLoggerFunction();

      virtual std::vector<char> readFile(const std::string& path);

      virtual bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);

      virtual void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& object, Logger& logger);

      virtual QueueFamilyIndexInfo findQueueFamilies(
          VkPhysicalDevice& physicalDevice,
          bool computeFamilyRequired,
          bool transferFamilyRequired,
          bool graphicsFamilyRequired,
          const VkSurfaceKHR& surface
      );

      virtual bool checkDeviceExtensionSupport(VkPhysicalDevice& physicalDevice, const std::vector<const char*>& deviceExtensions);

      virtual SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);

      virtual VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats);

      virtual VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes);

      virtual VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities, const uint32_t& width, const uint32_t& height);

      virtual uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

      virtual VkFormat findSupportedFormat(VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

      virtual VkFormat findDepthFormat(VkPhysicalDevice& physicalDevice);

      // create

      virtual std::map<std::string, std::string> createEnvironmentVariables(const std::string& executableDirPath);

      virtual Configuration createConfiguration();

      virtual Logger createLogger();
      virtual Logger createLogger(std::ostream& stream);
      virtual Logger createLogger(
          VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
          VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity
      );
      virtual Logger createLogger(
          VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
          VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity,
          std::ostream& stream
      );
      virtual Logger createLogger(
          const std::function<std::string(
              VkDebugUtilsMessageSeverityFlagBitsEXT,
              VkDebugUtilsMessageTypeFlagsEXT,
              const std::string&
          )>& function,
          VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
          VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity
      );
      virtual Logger createLogger(
          const std::function<std::string(
              VkDebugUtilsMessageSeverityFlagBitsEXT,
              VkDebugUtilsMessageTypeFlagsEXT,
              const std::string&
          )>& function,
          VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
          VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity,
          std::ostream& stream
      );

      virtual Functions createFunctions();

      virtual VkInstance createInstance(Configuration& configuration, Logger& logger);

      virtual VkDebugUtilsMessengerEXT createDebugUtilsMessenger(VkInstance& instance, Logger& logger);

      virtual VkSurfaceKHR createSurface(VkInstance& instance);

      virtual PhysicalDevice createPhysicalDevice(
          VkInstance& instance,
          Configuration& configuration
      );
      virtual PhysicalDevice createPhysicalDevice(
          VkInstance& instance,
          Configuration& configuration,
          VkSurfaceKHR& surface
      );

      virtual VkDevice createDevice(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          QueueFamilyIndexInfo& queueFamilyIndexInfo
      );

      virtual Queue createQueue(
          VkDevice& device,
          uint32_t queueFamilyIndex,
          uint32_t queueIndex
      );

      virtual VkCommandPool createCommandPool(
          VkDevice& device,
          uint32_t queueFamilyIndex
      );
      virtual VkCommandPool createCommandPool(
          VkDevice& device,
          uint32_t queueFamilyIndex,
          VkCommandPoolCreateFlags flags
      );

      virtual SwapChain createSwapChain(
          SwapChainSupportDetails& swapChainSupport,
          QueueFamilyIndexInfo& queueFamilyIndexInfo,
          VkSurfaceKHR& surface,
          VkDevice& device,
          const uint32_t& width,
          const uint32_t& height
      );

      virtual Buffer createBuffer(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          const BufferCreateInfo& createInfo,
          VkMemoryPropertyFlags properties
      );
      virtual Buffer createBuffer(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          VkDeviceSize memorySize,
          VkBufferUsageFlags usage,
          VkMemoryPropertyFlags properties
      );
      virtual std::vector<Buffer> createBuffers(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          VkDeviceSize memorySize,
          VkBufferUsageFlags usage,
          VkMemoryPropertyFlags properties,
          std::size_t size
      );

      virtual Image createImage(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          const ImageCreateInfo& createInfo,
          VkMemoryPropertyFlags properties
      );
      virtual Image createImage(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          uint32_t width,
          uint32_t height,
          VkFormat format,
          VkImageTiling tiling,
          VkImageUsageFlags usage,
          VkMemoryPropertyFlags properties
      );
      virtual std::vector<Image> createImages(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          uint32_t width,
          uint32_t height,
          VkFormat format,
          VkImageTiling tiling,
          VkImageUsageFlags usage,
          VkMemoryPropertyFlags properties,
          std::size_t size
      );

      virtual std::vector<VkImage> createSwapChainImages(
          VkDevice& device,
          VkSwapchainKHR& swapChain
      );

      virtual VkImageView createImageView(VkDevice& device, VkImage& image, VkFormat format);
      virtual VkImageView createImageView(VkDevice& device, VkImage& image, VkFormat format, VkImageAspectFlags aspectMask);
      virtual std::vector<VkImageView> createImageViews(VkDevice& device, std::vector<VkImage>& images, VkFormat format);
      virtual std::vector<VkImageView> createImageViews(VkDevice& device, std::vector<VkImage>& images, VkFormat format, VkImageAspectFlags aspectMask);

      virtual VkRenderPass createRenderPass(VkDevice& device, VkFormat& format);
      virtual VkRenderPass createRenderPass(VkDevice& device, const RenderPassCreateInfo& createInfo);

      virtual VkDescriptorSetLayout createDescriptorSetLayout(VkDevice& device);
      virtual VkDescriptorSetLayout createDescriptorSetLayout(VkDevice& device, const DescriptorSetLayoutCreateInfo& createInfo);

      virtual Shader createShader(VkDevice& device, const std::string& path);

      virtual Pipeline createPipeline(
          VkDevice& device,
          const std::vector<std::string>& shaderPaths,
          const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
          const std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions,
          const std::vector<VkVertexInputAttributeDescription>& vertexAttributeDescriptions,
          const PipelineLayoutCreateInfo& layoutCreateInfo,
          const GraphicsPipelineCreateInfo& createInfo
      );

      virtual Pipeline createGraphicsPipeline(
          VkDevice& device,
          const VkExtent2D& extent,
          const std::vector<std::string>& shaderPaths,
          VkRenderPass& renderPass
      );
      virtual Pipeline createGraphicsPipeline(
          VkDevice& device,
          const VkExtent2D& extent,
          const std::vector<std::string>& shaderPaths,
          VkRenderPass& renderPass,
          VkFrontFace frontFace,
          std::vector<VkDescriptorSetLayout> descriptorSetLayouts,
          std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions,
          std::vector<VkVertexInputAttributeDescription> attributeDescriptions
      );

      virtual VkFramebuffer createFrameBuffer(VkDevice& device, const FrameBufferCreateInfo& createInfo);
      virtual std::vector<VkFramebuffer> createFrameBuffers(
          VkDevice& device,
          const std::vector<FrameBufferCreateInfo>& createInfo
      );

      virtual VkSampler createSampler(VkPhysicalDevice& physicalDevice, VkDevice& device);

      virtual VkDescriptorPool createDescriptorPool(VkDevice& device, uint32_t maxSets);
      virtual VkDescriptorPool createDescriptorPool(VkDevice& device, const DescriptorPoolCreateInfo& createInfo);

      virtual VkDescriptorSet createDescriptorSet(
          VkDevice& device,
          VkDescriptorPool& descriptorPool,
          VkDescriptorSetLayout& descriptorSetLayout,
          const std::vector<WriteDescriptorSet>& writeDescriptorSets
      );

      virtual VkCommandBuffer createCommandBuffer(VkDevice& device, VkCommandPool& commandPool);
      virtual std::vector<VkCommandBuffer> createCommandBuffers(VkDevice& device, VkCommandPool& commandPool, std::size_t size);

      virtual VkSemaphore createSemaphore(VkDevice& device);
      virtual VkSemaphore createSemaphore(VkDevice& device, VkSemaphoreCreateFlags flags);
      virtual std::vector<VkSemaphore> createSemaphores(VkDevice& device, std::size_t size);
      virtual std::vector<VkSemaphore> createSemaphores(VkDevice& device, VkSemaphoreCreateFlags flags, std::size_t size);

      virtual VkFence createFence(VkDevice& device);
      virtual VkFence createFence(VkDevice& device, VkFenceCreateFlags flags);
      virtual std::vector<VkFence> createFences(VkDevice& device, std::size_t size);
      virtual std::vector<VkFence> createFences(VkDevice& device, VkFenceCreateFlags flags, std::size_t size);

      // destroy

      virtual void destroyFence(VkFence& fence, VkDevice& device);
      virtual void destroyFences(std::vector<VkFence>& fences, VkDevice& device);

      virtual void destroySemaphore(VkSemaphore& semaphore, VkDevice& device);
      virtual void destroySemaphores(std::vector<VkSemaphore>& semaphores, VkDevice& device);

      virtual void destroyCommandBuffer(VkCommandBuffer& commandBuffer, VkCommandPool& commandPool, VkDevice& device);
      virtual void destroyCommandBuffers(std::vector<VkCommandBuffer>& commandBuffers, VkCommandPool& commandPool, VkDevice& device);

      virtual void destroyDescriptorSet(VkDescriptorSet& descriptorSet);
      virtual void destroyDescriptorSets(std::vector<VkDescriptorSet>& descriptorSets);

      virtual void destroyDescriptorPool(VkDescriptorPool& descriptorPool, VkDevice& device);

      virtual void destroySampler(VkSampler& sampler, VkDevice& device);

      virtual void destroyFrameBuffer(VkFramebuffer& frameBuffer, VkDevice& device);
      virtual void destroyFrameBuffers(std::vector<VkFramebuffer>& frameBuffers, VkDevice& device);

      virtual void destroyPipeline(Pipeline& pipeline, VkDevice& device);

      virtual void destroyShader(Shader& shader, VkDevice& device);

      virtual void destroyDescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout, VkDevice& device);

      virtual void destroyRenderPass(VkRenderPass& renderPass, VkDevice& device);

      virtual void destroyImageView(VkImageView& imageView, VkDevice& device);
      virtual void destroyImageViews(std::vector<VkImageView>& imageViews, VkDevice& device);

      virtual void destroyImage(Image& image, VkDevice& device);
      virtual void destroyImage(Image& image, VkDevice& device, bool unmapMemory);
      virtual void destroyImages(std::vector<Image>& images, VkDevice& device);
      virtual void destroyImages(std::vector<Image>& images, VkDevice& device, bool unmapMemory);

      virtual void destroyBuffer(Buffer& buffer, VkDevice& device);
      virtual void destroyBuffer(Buffer& buffer, VkDevice& device, bool unmapMemory);
      virtual void destroyBuffers(std::vector<Buffer>& buffers, VkDevice& device);
      virtual void destroyBuffers(std::vector<Buffer>& buffers, VkDevice& device, bool unmapMemory);

      virtual void destroySwapChain(SwapChain& swapChain, VkDevice& device);

      virtual void destroyCommandPool(VkCommandPool& commandPool, VkDevice& device);

      virtual void destroyQueue(Queue& queue);

      virtual void destroyDevice(VkDevice& device);

      virtual void destroyPhysicalDevice(PhysicalDevice& physicalDevice);

      virtual void destroySurface(VkSurfaceKHR& surface, VkInstance& instance);

      virtual void destroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT& debugUtilsMessenger, VkInstance& instance);

      virtual void destroyInstance(VkInstance& instance);

  };

}
