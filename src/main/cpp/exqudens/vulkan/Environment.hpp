#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>
#include <optional>
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <limits>
#include <utility>
#include <algorithm>
#include <ostream>

#include "exqudens/vulkan/Macros.hpp"
#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/Context.hpp"
#include "exqudens/vulkan/Instance.hpp"
#include "exqudens/vulkan/Messenger.hpp"
#include "exqudens/vulkan/DebugUtilsMessenger.hpp"
#include "exqudens/vulkan/Surface.hpp"
#include "exqudens/vulkan/PhysicalDevice.hpp"
#include "exqudens/vulkan/Device.hpp"
#include "exqudens/vulkan/SwapChain.hpp"
#include "exqudens/vulkan/Queue.hpp"
#include "exqudens/vulkan/Image.hpp"
#include "exqudens/vulkan/ImageView.hpp"
#include "exqudens/vulkan/Buffer.hpp"
#include "exqudens/vulkan/Sampler.hpp"
#include "exqudens/vulkan/Semaphore.hpp"
#include "exqudens/vulkan/Fence.hpp"
#include "exqudens/vulkan/RenderPass.hpp"
#include "exqudens/vulkan/DescriptorSetLayout.hpp"
#include "exqudens/vulkan/Pipeline.hpp"

namespace exqudens::vulkan {

  class Environment: public Utility {

    protected:

      unsigned int contextId = 1;
      unsigned int instanceId = 1;
      unsigned int messengerId = 1;
      unsigned int debugUtilsMessengerId = 1;
      unsigned int physicalDeviceId = 1;
      unsigned int deviceId = 1;
      unsigned int queueId = 1;
      unsigned int imageId = 1;
      unsigned int imageViewId = 1;
      unsigned int bufferId = 1;
      unsigned int samplerId = 1;
      unsigned int semaphoreId = 1;
      unsigned int fenceId = 1;
      unsigned int renderPassId = 1;
      unsigned int descriptorSetLayoutId = 1;
      unsigned int pipelineId = 1;
      unsigned int surfaceId = 1;
      unsigned int swapChainId = 1;

      std::vector<vk::raii::PhysicalDevice> physicalDevices = {};

      std::map<unsigned int, std::shared_ptr<Context>> contextMap = {};
      std::map<unsigned int, std::shared_ptr<Instance>> instanceMap = {};
      std::map<unsigned int, std::shared_ptr<Messenger>> messengerMap = {};
      std::map<unsigned int, std::shared_ptr<DebugUtilsMessenger>> debugUtilsMessengerMap = {};
      std::map<unsigned int, std::shared_ptr<PhysicalDevice>> physicalDeviceMap = {};
      std::map<unsigned int, std::shared_ptr<Device>> deviceMap = {};
      std::map<unsigned int, std::shared_ptr<Queue>> queueMap = {};
      std::map<unsigned int, std::shared_ptr<Image>> imageMap = {};
      std::map<unsigned int, std::shared_ptr<ImageView>> imageViewMap = {};
      std::map<unsigned int, std::shared_ptr<Buffer>> bufferMap = {};
      std::map<unsigned int, std::shared_ptr<Sampler>> samplerMap = {};
      std::map<unsigned int, std::shared_ptr<Semaphore>> semaphoreMap = {};
      std::map<unsigned int, std::shared_ptr<Fence>> fenceMap = {};
      std::map<unsigned int, std::shared_ptr<RenderPass>> renderPassMap = {};
      std::map<unsigned int, std::shared_ptr<DescriptorSetLayout>> descriptorSetLayoutMap = {};
      std::map<unsigned int, std::shared_ptr<Pipeline>> pipelineMap = {};
      std::map<unsigned int, std::shared_ptr<Surface>> surfaceMap = {};
      std::map<unsigned int, std::shared_ptr<SwapChain>> swapChainMap = {};

    public:

      virtual Context createContext(
          const ContextCreateInfo& createInfo
      ) {
        try {
          for (auto const& [key, value] : createInfo.environmentVariables) {
            setEnvironmentVariable(std::string(key), std::string(value));
          }

          auto* value = new Context;
          value->id = contextId++;
          value->createInfo = createInfo;
          value->value = std::make_shared<vk::raii::Context>();
          contextMap[value->id] = std::shared_ptr<Context>(value);
          return *contextMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual Instance createInstance(
          Context& context,
          const InstanceCreateInfo& createInfo
      ) {
        try {
          auto* value = new Instance;
          value->id = instanceId++;
          value->createInfo = createInfo;
          value->value = std::make_shared<vk::raii::Instance>(
              context.reference(),
              value->createInfo
          );
          instanceMap[value->id] = std::shared_ptr<Instance>(value);
          return *instanceMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual Messenger createMessenger(
          std::ostream& out,
          const std::optional<vk::DebugUtilsMessageSeverityFlagsEXT>& exceptionSeverity,
          const std::optional<vk::DebugUtilsMessageSeverityFlagsEXT>& outSeverity,
          const std::function<std::string(
              vk::DebugUtilsMessageSeverityFlagsEXT,
              vk::DebugUtilsMessageTypeFlagsEXT,
              std::string
          )>& toStringFunction
      ) {
        try {
          auto* value = new Messenger;
          value->id = messengerId++;
          value->exceptionSeverity = exceptionSeverity;
          value->outSeverity = outSeverity;
          if (toStringFunction) {
            value->toStringFunction = toStringFunction;
          } else {
            /*value->toStringFunction = std::bind(
                &Environment::toString,
                this,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3
            );*/
            value->toStringFunction = [this](auto&& o1, auto&& o2, auto&& o3) {
              return toString(
                  std::forward<decltype(o1)>(o1),
                  std::forward<decltype(o2)>(o2),
                  std::forward<decltype(o3)>(o3)
              );
            };
          }
          value->out = &out;
          messengerMap[value->id] = std::shared_ptr<Messenger>(value);
          return *messengerMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual DebugUtilsMessenger createDebugUtilsMessenger(
          Instance& instance,
          Messenger& messenger,
          const vk::DebugUtilsMessengerCreateInfoEXT& createInfo
      ) {
        try {
          auto* value = new DebugUtilsMessenger;
          value->id = debugUtilsMessengerId++;
          value->createInfo = createInfo;
          value->value = std::make_shared<vk::raii::DebugUtilsMessengerEXT>(
              instance.reference(),
              createInfo
          );
          debugUtilsMessengerMap[value->id] = std::shared_ptr<DebugUtilsMessenger>(value);
          return *debugUtilsMessengerMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual PhysicalDevice createPhysicalDevice(
          Instance& instance,
          const PhysicalDeviceCreateInfo& createInfo
      ) {
        try {
          auto* value = new PhysicalDevice;
          value->id = physicalDeviceId++;
          value->createInfo = createInfo;
          value->value = nullptr;
          physicalDevices = vk::raii::PhysicalDevices(*instance.value);
          for (vk::raii::PhysicalDevice& physicalDevice : physicalDevices) {

            bool queueFamilyIndicesAdequate = true;
            bool deviceExtensionAdequate = true;
            bool swapChainAdequate = true;
            bool anisotropyAdequate = true;

            std::vector<vk::DeviceQueueCreateInfo> computeQueueCreateInfos = {};
            std::vector<vk::DeviceQueueCreateInfo> transferQueueCreateInfos = {};
            std::vector<vk::DeviceQueueCreateInfo> graphicsQueueCreateInfos = {};
            std::vector<vk::DeviceQueueCreateInfo> presentQueueCreateInfos = {};
            std::map<uint32_t, vk::DeviceQueueCreateInfo> queueCreateInfoMap = {};

            std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
            for (const vk::QueueFlagBits& queueType : value->createInfo.queueTypes) {
              std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = {};
              for (std::size_t i = 0; i < queueFamilyProperties.size(); i++) {
                if (queueFamilyProperties[i].queueFlags & queueType) {
                  vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo()
                      .setQueueFamilyIndex(static_cast<uint32_t>(i))
                      .setQueueCount(queueFamilyProperties[i].queueCount)
                      .setFlags({})
                      .setQueuePriorities(value->createInfo.queuePriorities);
                  queueCreateInfos.emplace_back(queueCreateInfo);
                  queueCreateInfoMap.try_emplace(queueCreateInfo.queueFamilyIndex, queueCreateInfo);
                }
              }
              if (queueCreateInfos.empty()) {
                queueFamilyIndicesAdequate = false;
                break;
              } else {
                if (vk::QueueFlagBits::eCompute == queueType) {
                  computeQueueCreateInfos = queueCreateInfos;
                } else if (vk::QueueFlagBits::eTransfer == queueType) {
                  transferQueueCreateInfos = queueCreateInfos;
                } else if (vk::QueueFlagBits::eGraphics == queueType) {
                  graphicsQueueCreateInfos = queueCreateInfos;
                }
              }
            }
            if (!queueFamilyIndicesAdequate) {
              continue;
            }
            if (value->createInfo.surface) {
              std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = {};
              for (std::size_t i = 0; i < queueFamilyProperties.size(); i++) {
                if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), value->createInfo.surface.value())) {
                  vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo()
                      .setQueueFamilyIndex(static_cast<uint32_t>(i))
                      .setQueueCount(queueFamilyProperties[i].queueCount)
                      .setFlags({})
                      .setQueuePriorities(value->createInfo.queuePriorities);
                  queueCreateInfos.emplace_back(queueCreateInfo);
                  queueCreateInfoMap.try_emplace(queueCreateInfo.queueFamilyIndex, queueCreateInfo);
                }
              }
              if (queueCreateInfos.empty()) {
                queueFamilyIndicesAdequate = false;
              } else {
                presentQueueCreateInfos = queueCreateInfos;
              }
            }
            if (!queueFamilyIndicesAdequate) {
              continue;
            }

            std::vector<vk::ExtensionProperties> extensionProperties = physicalDevice.enumerateDeviceExtensionProperties(nullptr);
            std::set<std::string> requiredExtensions(value->createInfo.enabledExtensionNames.begin(), value->createInfo.enabledExtensionNames.end());
            for (const auto& extension : extensionProperties) {
              std::string name = std::string(extension.extensionName.data());
              requiredExtensions.erase(name);
            }
            deviceExtensionAdequate = requiredExtensions.empty();
            if (!deviceExtensionAdequate) {
              continue;
            }

            if (value->createInfo.surface) {
              std::vector<vk::SurfaceFormatKHR> formats = physicalDevice.getSurfaceFormatsKHR(value->createInfo.surface.value());
              std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(value->createInfo.surface.value());
              swapChainAdequate = !formats.empty() && !presentModes.empty();
            }
            if (!swapChainAdequate) {
              continue;
            }

            if (value->createInfo.features.samplerAnisotropy) {
              vk::PhysicalDeviceFeatures physicalDeviceFeatures = physicalDevice.getFeatures();
              anisotropyAdequate = physicalDeviceFeatures.samplerAnisotropy;
            }
            if (!anisotropyAdequate) {
              continue;
            }

            value->computeQueueCreateInfos = computeQueueCreateInfos;
            value->transferQueueCreateInfos = transferQueueCreateInfos;
            value->graphicsQueueCreateInfos = graphicsQueueCreateInfos;
            value->presentQueueCreateInfos = presentQueueCreateInfos;
            std::vector<vk::DeviceQueueCreateInfo> uniqueQueueCreateInfos;
            for (const auto& [k, v] : queueCreateInfoMap) {
              uniqueQueueCreateInfos.emplace_back(v);
            }
            value->uniqueQueueCreateInfos = uniqueQueueCreateInfos;
            value->value = &physicalDevice;
            break;
          }
          if (value->value == nullptr) {
            throw std::runtime_error(CALL_INFO() + ": failed to create physical device!");
          }
          physicalDeviceMap[value->id] = std::shared_ptr<PhysicalDevice>(value);
          return *physicalDeviceMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual Device createDevice(
          PhysicalDevice& physicalDevice,
          const vk::DeviceCreateInfo& createInfo
      ) {
        try {
          auto* value = new Device;
          value->id = deviceId++;
          value->createInfo = createInfo;
          value->value = std::make_shared<vk::raii::Device>(
              physicalDevice.reference(),
              value->createInfo
          );
          deviceMap[value->id] = std::shared_ptr<Device>(value);
          return *deviceMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual Queue createQueue(
          Device& device,
          const uint32_t& queueFamilyIndex,
          const uint32_t& queueIndex
      ) {
        try {
          auto* value = new Queue;
          value->id = queueId++;
          value->familyIndex = queueFamilyIndex;
          value->index = queueIndex;
          value->value = std::make_shared<vk::raii::Queue>(
              device.reference(),
              value->familyIndex,
              value->index
          );
          queueMap[value->id] = std::shared_ptr<Queue>(value);
          return *queueMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual Image createImage(
          PhysicalDevice& physicalDevice,
          Device& device,
          const vk::ImageCreateInfo& createInfo,
          const vk::MemoryPropertyFlags& properties
      ) {
        try {
          auto* value = new Image;
          value->id = imageId++;
          value->createInfo = createInfo;
          value->value = std::make_shared<vk::raii::Image>(
              device.reference(),
              value->createInfo
          );
          vk::MemoryRequirements memoryRequirements = value->value->getMemoryRequirements();
          uint32_t memoryType = memoryTypeIndex(physicalDevice, memoryRequirements.memoryTypeBits, properties);
          value->size = memoryRequirements.size;
          value->memory = std::make_shared<vk::raii::DeviceMemory>(
              device.reference(),
              vk::MemoryAllocateInfo()
                  .setAllocationSize(value->size)
                  .setMemoryTypeIndex(memoryType)
          );
          value->value->bindMemory(*(*value->memory), 0);
          imageMap[value->id] = std::shared_ptr<Image>(value);
          return *imageMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual ImageView createImageView(
          Device& device,
          const vk::ImageViewCreateInfo& createInfo
      ) {
        try {
          auto* value = new ImageView;
          value->id = imageViewId++;
          value->createInfo = createInfo;
          value->value = std::make_shared<vk::raii::ImageView>(
              device.reference(),
              value->createInfo
          );
          imageViewMap[value->id] = std::shared_ptr<ImageView>(value);
          return *imageViewMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual Buffer createBuffer(
          PhysicalDevice& physicalDevice,
          Device& device,
          const vk::BufferCreateInfo& createInfo,
          const vk::MemoryPropertyFlags& properties
      ) {
        try {
          auto* value = new Buffer;
          value->id = bufferId++;
          value->createInfo = createInfo;
          value->value = std::make_shared<vk::raii::Buffer>(
              device.reference(),
              value->createInfo
          );
          vk::MemoryRequirements memoryRequirements = value->value->getMemoryRequirements();
          uint32_t memoryType = memoryTypeIndex(physicalDevice, memoryRequirements.memoryTypeBits, properties);
          value->size = memoryRequirements.size;
          value->memory = std::make_shared<vk::raii::DeviceMemory>(
              device.reference(),
              vk::MemoryAllocateInfo()
                  .setAllocationSize(value->size)
                  .setMemoryTypeIndex(memoryType)
          );
          value->value->bindMemory(*(*value->memory), 0);
          bufferMap[value->id] = std::shared_ptr<Buffer>(value);
          return *bufferMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual Sampler createSampler(
          Device& device,
          const vk::SamplerCreateInfo& createInfo
      ) {
        try {
          auto* value = new Sampler;
          value->id = samplerId++;
          value->createInfo = createInfo;
          value->value = std::make_shared<vk::raii::Sampler>(
              device.reference(),
              value->createInfo
          );
          samplerMap[value->id] = std::shared_ptr<Sampler>(value);
          return *samplerMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual Semaphore createSemaphore(
          Device& device,
          const vk::SemaphoreCreateInfo& createInfo
          ) {
        try {
          auto* value = new Semaphore;
          value->id = semaphoreId++;
          value->createInfo = createInfo;
          value->value = std::make_shared<vk::raii::Semaphore>(
              device.reference(),
              value->createInfo
          );
          semaphoreMap[value->id] = std::shared_ptr<Semaphore>(value);
          return *semaphoreMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual Fence createFence(
          Device& device,
          const vk::FenceCreateInfo& createInfo
      ) {
        try {
          auto* value = new Fence;
          value->id = fenceId++;
          value->value = std::make_shared<vk::raii::Fence>(
              device.reference(),
              value->createInfo
          );
          fenceMap[value->id] = std::shared_ptr<Fence>(value);
          return *fenceMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual RenderPass createRenderPass(
          Device& device,
          const RenderPassCreateInfo& createInfo
      ) {
        try {
          auto* value = new RenderPass;
          value->id = renderPassId++;
          value->createInfo = createInfo;
          value->value = std::make_shared<vk::raii::RenderPass>(
              device.reference(),
              value->createInfo
          );
          renderPassMap[value->id] = std::shared_ptr<RenderPass>(value);
          return *renderPassMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual DescriptorSetLayout createDescriptorSetLayout(
          Device& device,
          const vk::DescriptorSetLayoutCreateFlags& flags,
          const std::vector<vk::DescriptorSetLayoutBinding>& bindings
      ) {
        try {
          auto* value = new DescriptorSetLayout;
          value->id = descriptorSetLayoutId++;
          value->createInfo = vk::DescriptorSetLayoutCreateInfo()
              .setFlags(value->flags)
              .setBindings(value->bindings);
          value->value = std::make_shared<vk::raii::DescriptorSetLayout>(
              device.reference(),
              value->createInfo
          );
          descriptorSetLayoutMap[value->id] = std::shared_ptr<DescriptorSetLayout>(value);
          return *descriptorSetLayoutMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual Pipeline createPipeline(
          Device& device,
          const vk::PipelineCacheCreateInfo& cacheCreateInfo,
          const vk::PipelineLayoutCreateInfo& layoutCreateInfo,
          const std::vector<std::string>& paths,
          const vk::GraphicsPipelineCreateInfo& createInfo
      ) {
        try {
          auto* value = new Pipeline;
          value->id = pipelineId++;
          value->cacheCreateInfo = cacheCreateInfo;
          value->cache = std::make_shared<vk::raii::PipelineCache>(
              device.reference(),
              value->cacheCreateInfo
          );
          value->layoutCreateInfo = layoutCreateInfo;
          value->layout = std::make_shared<vk::raii::PipelineLayout>(
              device.reference(),
              value->layoutCreateInfo
          );
          value->graphicsPipelineCreateInfo = createInfo;

          std::vector<vk::PipelineShaderStageCreateInfo> stages;
          for (const std::string& path : paths) {
            if (!value->shaders.contains(path)) {
              std::vector<char> bytes = readFile(path);
              if (bytes.empty()) {
                throw std::runtime_error(CALL_INFO() + ": '" + path + "' failed to create shader module bytes is empty!");
              }
              vk::ShaderModuleCreateInfo shaderCreateInfo = vk::ShaderModuleCreateInfo()
                  .setCodeSize(bytes.size())
                  .setPCode(reinterpret_cast<const uint32_t*>(bytes.data()));
              value->shaders[path] = std::make_pair(
                  shaderCreateInfo,
                  std::make_shared<vk::raii::ShaderModule>(device.reference(), shaderCreateInfo)
              );
              vk::PipelineShaderStageCreateInfo stage = vk::PipelineShaderStageCreateInfo();
              stage.setPName("main");
              stage.setModule(*(*value->shaders[path].second));
              if (path.ends_with(".vert.spv")) {
                stage.setStage(vk::ShaderStageFlagBits::eVertex);
              } else if (path.ends_with(".frag.spv")) {
                stage.setStage(vk::ShaderStageFlagBits::eFragment);
              } else {
                throw std::invalid_argument(CALL_INFO() + ": '" + path + "' failed to create shader!");
              }
              stages.emplace_back(stage);
            }
          }
          value->graphicsPipelineCreateInfo.value().setStages(stages);

          value->value = std::make_shared<vk::raii::Pipeline>(
              device.reference(),
              value->cacheReference(),
              //vk::ComputePipelineCreateInfo()
              value->graphicsPipelineCreateInfo.value()
              //vk::RayTracingPipelineCreateInfoNV()
          );
          pipelineMap[value->id] = std::shared_ptr<Pipeline>(value);
          return *pipelineMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual Surface createSurface(
          Instance& instance,
          VkSurfaceKHR& vkSurface
      ) {
        try {
          auto* value = new Surface;
          value->id = surfaceId++;
          value->value = std::make_shared<vk::raii::SurfaceKHR>(*instance.value, vkSurface);
          surfaceMap[value->id] = std::shared_ptr<Surface>(value);
          return *surfaceMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual SwapChain createSwapChain(
          Device& device,
          const vk::SwapchainCreateInfoKHR& createInfo
      ) {
        try {
          auto* value = new SwapChain;
          value->id = swapChainId++;
          value->createInfo = createInfo;
          value->value = std::make_shared<vk::raii::SwapchainKHR>(
              *device.value,
              value->createInfo
          );
          swapChainMap[value->id] = std::shared_ptr<SwapChain>(value);
          return *swapChainMap[value->id];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      virtual std::vector<ImageView> createSwapChainImageViews(
          Device& device,
          SwapChain& swapChain
      ) {
        try {
          std::vector<ImageView> values;
          for (VkImage vkImage : swapChain.value->getImages()) {
            vk::ImageViewCreateInfo createInfo = vk::ImageViewCreateInfo()
                .setFlags({})
                .setImage(static_cast<vk::Image>(vkImage))
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(swapChain.createInfo.imageFormat)
                .setComponents({})
                .setSubresourceRange(
                    vk::ImageSubresourceRange()
                        .setAspectMask(vk::ImageAspectFlagBits::eColor)
                        .setBaseMipLevel(0)
                        .setLevelCount(1)
                        .setBaseArrayLayer(0)
                        .setLayerCount(1)
                );
            ImageView imageView = createImageView(device, createInfo);
            values.emplace_back(imageView);
          }
          return values;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
