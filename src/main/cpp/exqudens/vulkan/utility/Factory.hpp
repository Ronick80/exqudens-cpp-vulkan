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
#include "exqudens/vulkan/utility/Logger.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"
#include "exqudens/vulkan/model/QueueFamilyIndexInfo.hpp"
#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"
#include "exqudens/vulkan/model/PhysicalDevice.hpp"
#include "exqudens/vulkan/model/SwapChain.hpp"
#include "exqudens/vulkan/model/Buffer.hpp"
#include "exqudens/vulkan/model/Image.hpp"
#include "exqudens/vulkan/model/Shader.hpp"
#include "exqudens/vulkan/model/Pipeline.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT Factory {

    public:

      std::function<VkSurfaceKHR(VkInstance&)> createSurfaceFunction;

      explicit Factory(
          std::function<VkSurfaceKHR(VkInstance&)> createSurfaceFunction
      );
      Factory();
      Factory(const Factory& object);
      Factory(Factory&& object) noexcept;

      Factory& operator=(const Factory& object);
      Factory& operator=(Factory&& object) noexcept;

      friend void swap(Factory& first, Factory& second);

      // utility

      virtual void setEnvironmentVariable(const std::string& name, const std::string& value);

      virtual std::optional<std::string> getEnvironmentVariable(const std::string& name);

      virtual std::vector<char> readFile(const std::string& path);

      virtual bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);

      virtual void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& object, Logger& logger);

      virtual QueueFamilyIndexInfo findQueueFamilies(
          VkPhysicalDevice& physicalDevice,
          bool computeFamilyRequired,
          bool transferFamilyRequired,
          bool graphicsFamilyRequired,
          VkSurfaceKHR& surface
      );

      virtual bool checkDeviceExtensionSupport(VkPhysicalDevice& physicalDevice, const std::vector<const char*>& deviceExtensions);

      virtual SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);

      virtual VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats);

      virtual VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes);

      virtual VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities, const int& width, const int& height);

      virtual uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

      // create

      virtual std::map<std::string, std::string> createEnvironmentVariables(const std::string& executableDirPath);

      virtual Configuration createConfiguration();

      virtual Logger createLogger();
      virtual Logger createLogger(std::ostream& stream);
      virtual Logger createLogger(
          const std::function<void(
              VkDebugUtilsMessageSeverityFlagBitsEXT,
              VkDebugUtilsMessageTypeFlagsEXT,
              const std::string&
          )>& function
      );

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

      virtual VkQueue createQueue(
          VkDevice& device,
          uint32_t queueFamilyIndex,
          uint32_t queueIndex
      );

      virtual SwapChain createSwapChain(
          SwapChainSupportDetails& swapChainSupport,
          QueueFamilyIndexInfo& queueFamilyIndexInfo,
          VkSurfaceKHR& surface,
          VkDevice& device,
          const int& width,
          const int& height
      );

      virtual Buffer createBuffer(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          VkDeviceSize size,
          VkBufferUsageFlags usage,
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

      virtual VkImageView createImageView(VkDevice& device, VkImage& image, VkFormat& format);
      virtual std::vector<VkImageView> createImageViews(VkDevice& device, std::vector<VkImage>& images, VkFormat& format);

      virtual VkRenderPass createRenderPass(VkDevice& device, VkFormat& format);

      virtual VkDescriptorSetLayout createDescriptorSetLayout(VkDevice& device);

      virtual Shader createShader(VkDevice& device, const std::string& path);

      virtual Pipeline createGraphicsPipeline(
          VkDevice& device,
          VkExtent2D& extent,
          const std::vector<std::string>& shaderPaths,
          VkRenderPass& renderPass
      );
      virtual Pipeline createGraphicsPipeline(
          VkDevice& device,
          VkExtent2D& extent,
          const std::vector<std::string>& shaderPaths,
          VkRenderPass& renderPass,
          VkFrontFace frontFace,
          VkDescriptorSetLayout& descriptorSetLayout,
          std::optional<VkVertexInputBindingDescription> bindingDescription,
          std::vector<VkVertexInputAttributeDescription> attributeDescriptions
      );

      virtual VkFramebuffer createFrameBuffer(
          VkDevice& device,
          VkImageView& imageView,
          VkRenderPass& renderPass,
          const int& width,
          const int& height
      );
      virtual std::vector<VkFramebuffer> createFrameBuffers(
          VkDevice& device,
          std::vector<VkImageView>& imageViews,
          VkRenderPass& renderPass,
          const int& width,
          const int& height
      );

      virtual VkDescriptorPool createDescriptorPool(VkDevice& device, std::size_t size);

      virtual VkCommandPool createComputeCommandPool(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface,
          VkDevice& device
      );
      virtual VkCommandPool createTransferCommandPool(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface,
          VkDevice& device
      );
      virtual VkCommandPool createGraphicsCommandPool(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface,
          VkDevice& device
      );
      virtual VkCommandPool createPresentCommandPool(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface,
          VkDevice& device
      );

      virtual VkCommandBuffer createCommandBuffer(VkDevice& device, VkCommandPool& commandPool);
      virtual std::vector<VkCommandBuffer> createCommandBuffers(VkDevice& device, VkCommandPool& commandPool, std::size_t size);

      // destroy

      virtual void destroyCommandBuffer(VkCommandBuffer& commandBuffer, VkCommandPool& commandPool, VkDevice& device);
      virtual void destroyCommandBuffers(std::vector<VkCommandBuffer>& commandBuffers, VkCommandPool& commandPool, VkDevice& device);

      virtual void destroyCommandPool(VkCommandPool& commandPool, VkDevice& device);

      virtual void destroyDescriptorPool(VkDescriptorPool& descriptorPool, VkDevice& device);

      virtual void destroyFrameBuffer(VkFramebuffer& frameBuffer, VkDevice& device);
      virtual void destroyFrameBuffers(std::vector<VkFramebuffer>& frameBuffers, VkDevice& device);

      virtual void destroyPipeline(Pipeline& pipeline, VkDevice& device);
      virtual void destroyPipeline(VkPipeline& pipeline, VkDevice& device);

      virtual void destroyPipelineLayout(VkPipelineLayout& pipelineLayout, VkDevice& device);

      virtual void destroyShader(Shader& shader, VkDevice& device);

      virtual void destroyDescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout, VkDevice& device);

      virtual void destroyRenderPass(VkRenderPass& renderPass, VkDevice& device);

      virtual void destroyImageView(VkImageView& imageView, VkDevice& device);
      virtual void destroyImageViews(std::vector<VkImageView>& imageViews, VkDevice& device);

      virtual void destroyImage(Image& image, VkDevice& device);
      virtual void destroyImages(std::vector<Image>& images, VkDevice& device);

      virtual void destroyBuffer(Buffer& buffer, VkDevice& device);

      virtual void destroySwapChain(SwapChain& swapChain, VkDevice& device);
      virtual void destroySwapChain(VkSwapchainKHR& swapChain, VkDevice& device);

      virtual void destroyQueue(VkQueue& queue);

      virtual void destroyDevice(VkDevice& device);

      virtual void destroyPhysicalDevice(PhysicalDevice& physicalDevice);

      virtual void destroySurface(VkSurfaceKHR& surface, VkInstance& instance);

      virtual void destroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT& debugUtilsMessenger, VkInstance& instance);

      virtual void destroyInstance(VkInstance& instance);

      virtual ~Factory();

  };

}
