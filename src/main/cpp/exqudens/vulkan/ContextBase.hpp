#pragma once

#include <iostream>

#include "exqudens/vulkan/Context.hpp"
#include "exqudens/vulkan/FactoryBase.hpp"

namespace exqudens::vulkan {

  class ContextBase:
      virtual public Context,
      virtual public FactoryBase
  {

    public:

      std::vector<Instance> instances = {};
      std::vector<DebugUtilsMessenger> debugUtilsMessengers = {};
      std::vector<PhysicalDevice> physicalDevices = {};
      std::vector<Device> devices = {};
      std::vector<Buffer> buffers = {};
      std::vector<Image> images = {};
      std::vector<ImageView> imageViews = {};
      std::vector<Sampler> samplers = {};
      std::vector<RenderPass> renderPasses = {};
      std::vector<DescriptorSetLayout> descriptorSetLayouts = {};
      std::vector<DescriptorPool> descriptorPools = {};
      std::vector<DescriptorSet> descriptorSets = {};
      std::vector<Pipeline> pipelines = {};
      std::vector<FrameBuffer> frameBuffers = {};
      std::vector<Surface> surfaces = {};
      std::vector<SwapChain> swapChains = {};
      std::vector<Queue> queues = {};
      std::vector<CommandPool> commandPools = {};
      std::vector<CommandBuffer> commandBuffers = {};
      std::vector<Semaphore> semaphores = {};
      std::vector<Fence> fences = {};

      Instance createInstance(Configuration& configuration, Logger& logger) override {
        try {
          return instances.emplace_back(
              InstanceFactoryBase::createInstance(
                  configuration,
                  logger
              )
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      DebugUtilsMessenger createDebugUtilsMessenger(VkInstance& instance, Logger& logger) override {
        try {
          return debugUtilsMessengers.emplace_back(
              DebugUtilsMessengerFactoryBase::createDebugUtilsMessenger(
                  instance,
                  logger
              )
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      PhysicalDevice createPhysicalDevice(
          VkInstance& instance,
          Configuration& configuration,
          const VkSurfaceKHR& surface
      ) override {
        try {
          return physicalDevices.emplace_back(
              PhysicalDeviceFactoryBase::createPhysicalDevice(
                  instance,
                  configuration,
                  surface
              )
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Device createDevice(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          QueueFamilyIndexInfo& queueFamilyIndexInfo
      ) override {
        try {
          return devices.emplace_back(
              DeviceFactoryBase::createDevice(
                  physicalDevice,
                  configuration,
                  queueFamilyIndexInfo
              )
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroy() override {
        try {
          // destroy fences
          for (auto& value : fences) {
            destroyFence(value);
          }
          fences.clear();

          // destroy semaphores
          for (auto& value : semaphores) {
            destroySemaphore(value);
          }
          semaphores.clear();

          // destroy commandBuffers
          for (auto& value : commandBuffers) {
            destroyCommandBuffer(value);
          }
          commandBuffers.clear();

          // destroy commandPools
          for (auto& value : commandPools) {
            destroyCommandPool(value);
          }
          commandPools.clear();

          // destroy queues
          for (auto& value : queues) {
            destroyQueue(value);
          }
          queues.clear();

          // destroy swapChains
          for (auto& value : swapChains) {
            destroySwapChain(value);
          }
          swapChains.clear();

          // destroy surfaces
          for (auto& value : surfaces) {
            destroySurface(value);
          }
          surfaces.clear();

          // destroy frameBuffers
          for (auto& value : frameBuffers) {
            destroyFrameBuffer(value);
          }
          frameBuffers.clear();

          // destroy pipelines
          for (auto& value : pipelines) {
            destroyPipeline(value);
          }
          pipelines.clear();

          // destroy descriptorSets
          for (auto& value : descriptorSets) {
            destroyDescriptorSet(value);
          }
          descriptorSets.clear();

          // destroy descriptorPools
          for (auto& value : descriptorPools) {
            destroyDescriptorPool(value);
          }
          descriptorPools.clear();

          // destroy descriptorSetLayouts
          for (auto& value : descriptorSetLayouts) {
            destroyDescriptorSetLayout(value);
          }
          descriptorSetLayouts.clear();

          // destroy renderPasses
          for (auto& value : renderPasses) {
            destroyRenderPass(value);
          }
          renderPasses.clear();

          // destroy samplers
          for (auto& value : samplers) {
            destroySampler(value);
          }
          samplers.clear();

          // destroy imageViews
          for (auto& value : imageViews) {
            destroyImageView(value);
          }
          imageViews.clear();

          // destroy images
          for (auto& value : images) {
            destroyImage(value);
          }
          images.clear();

          // destroy buffers
          for (auto& value : buffers) {
            destroyBuffer(value);
          }
          buffers.clear();

          // destroy devices
          for (auto& value : devices) {
            std::cout << "destroyDevice" << std::endl;
            destroyDevice(value);
          }
          devices.clear();

          // destroy physicalDevices
          for (auto& value : physicalDevices) {
            std::cout << "destroyPhysicalDevice" << std::endl;
            destroyPhysicalDevice(value);
          }
          physicalDevices.clear();

          // destroy debugUtilsMessengers
          for (auto& value : debugUtilsMessengers) {
            std::cout << "destroyDebugUtilsMessenger" << std::endl;
            destroyDebugUtilsMessenger(value);
          }
          debugUtilsMessengers.clear();

          // destroy instances
          for (auto& value : instances) {
            std::cout << "destroyInstance" << std::endl;
            destroyInstance(value);
          }
          instances.clear();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
