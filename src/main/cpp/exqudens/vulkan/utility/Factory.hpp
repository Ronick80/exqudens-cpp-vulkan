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
#include "exqudens/vulkan/model/QueueFamilyIndices.hpp"
#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"
#include "exqudens/vulkan/model/SwapChain.hpp"
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

      virtual bool isDeviceSuitable(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface
      );

      virtual QueueFamilyIndices findQueueFamilies(
          VkPhysicalDevice& physicalDevice,
          bool computeFamilyRequired,
          bool transferFamilyRequired,
          bool graphicsFamilyRequired,
          VkSurfaceKHR& surface
      );

      virtual bool isQueueFamilyIndicesComplete(QueueFamilyIndices& value);

      virtual std::set<uint32_t> uniqueQueueFamilyIndices(QueueFamilyIndices& value);

      virtual bool checkDeviceExtensionSupport(VkPhysicalDevice& physicalDevice, const std::vector<const char*>& deviceExtensions);

      virtual SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);

      virtual VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats);

      virtual VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes);

      virtual VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities, const int& width, const int& height);

      virtual uint32_t getSwapChainImageCount(SwapChainSupportDetails& swapChainSupport);

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

      virtual VkPhysicalDevice createPhysicalDevice(
          VkInstance& instance,
          Configuration& configuration,
          VkSurfaceKHR& surface
      );

      virtual VkDevice createDevice(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface
      );

      virtual VkQueue createComputeQueue(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface,
          VkDevice& device,
          uint32_t queueIndex
      );

      virtual VkQueue createTransferQueue(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface,
          VkDevice& device,
          uint32_t queueIndex
      );

      virtual VkQueue createGraphicsQueue(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface,
          VkDevice& device,
          uint32_t queueIndex
      );

      virtual VkQueue createPresentQueue(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface,
          VkDevice& device,
          uint32_t queueIndex
      );

      virtual SwapChain createSwapChain(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          VkSurfaceKHR& surface,
          VkDevice& device,
          const int& width,
          const int& height
      );

      virtual std::vector<VkImage> createSwapChainImages(
          VkPhysicalDevice& physicalDevice,
          VkSurfaceKHR& surface,
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
          VkDescriptorSetLayout& descriptorSetLayout,
          const std::vector<std::string>& shaderPaths,
          VkRenderPass& renderPass
      );

      // destroy

      virtual void destroyPipeline(Pipeline& pipeline, VkDevice& device);
      virtual void destroyPipeline(VkPipeline& pipeline, VkDevice& device);

      virtual void destroyPipelineLayout(VkPipelineLayout& pipelineLayout, VkDevice& device);

      virtual void destroyShader(Shader& shader, VkDevice& device);

      virtual void destroyDescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout, VkDevice& device);

      virtual void destroyRenderPass(VkRenderPass& renderPass, VkDevice& device);

      virtual void destroyImageView(VkImageView& imageView, VkDevice& device);
      virtual void destroyImageViews(std::vector<VkImageView>& imageViews, VkDevice& device);

      virtual void destroySwapChain(SwapChain& swapChain, VkDevice& device);
      virtual void destroySwapChain(VkSwapchainKHR& swapChain, VkDevice& device);

      virtual void destroyDevice(VkDevice& device);

      virtual void destroyPhysicalDevice(VkPhysicalDevice& physicalDevice);

      virtual void destroySurface(VkSurfaceKHR& surface, VkInstance& instance);

      virtual void destroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT& debugUtilsMessenger, VkInstance& instance);

      virtual void destroyInstance(VkInstance& instance);

      virtual ~Factory();

  };

}
