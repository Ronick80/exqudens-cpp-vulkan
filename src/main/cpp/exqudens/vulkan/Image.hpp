#pragma once

#include <optional>
#include <memory>
#include <functional>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan {

  struct Image {

    class Builder;

    static Builder builder();

    vk::ImageCreateInfo createInfo;
    std::shared_ptr<vk::raii::Image> value;
    vk::MemoryPropertyFlags memoryCreateInfo;
    std::shared_ptr<vk::raii::DeviceMemory> memory;

    vk::raii::Image& reference() {
      try {
        if (!value) {
          throw std::runtime_error(CALL_INFO() + ": value is not initialized!");
        }
        return *value;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

    vk::raii::DeviceMemory& memoryReference() {
      try {
        if (!memory) {
          throw std::runtime_error(CALL_INFO() + ": memory is not initialized!");
        }
        return *memory;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

  };

  class Image::Builder {

    private:

      std::weak_ptr<vk::raii::PhysicalDevice> physicalDevice;
      std::weak_ptr<vk::raii::Device> device;
      std::function<uint32_t(
          vk::raii::PhysicalDevice&,
          const uint32_t&,
          const vk::MemoryPropertyFlags&
      )> memoryTypeIndexFunction;
      std::optional<vk::ImageCreateInfo> createInfo;
      std::optional<vk::MemoryPropertyFlags> memoryCreateInfo;

    public:

      Image::Builder& setPhysicalDevice(const std::weak_ptr<vk::raii::PhysicalDevice>& val) {
        physicalDevice = val;
        return *this;
      }

      Image::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      Image::Builder& setMemoryTypeIndexFunction(
          const std::function<uint32_t(
              vk::raii::PhysicalDevice&,
              const uint32_t&,
              const vk::MemoryPropertyFlags&
          )>& val
      ) {
        memoryTypeIndexFunction = val;
        return *this;
      }

      Image::Builder& setCreateInfo(const vk::ImageCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      Image::Builder& setMemoryCreateInfo(const vk::MemoryPropertyFlags& val) {
        memoryCreateInfo = val;
        return *this;
      }

      Image build() {
        try {
          if (!memoryTypeIndexFunction) {
            memoryTypeIndexFunction = &Utility::memoryTypeIndex;
          }

          Image target = {};
          target.createInfo = createInfo.value();
          target.value = std::make_shared<vk::raii::Image>(
              *device.lock(),
              target.createInfo
          );
          target.memoryCreateInfo = memoryCreateInfo.value();
          vk::MemoryRequirements memoryRequirements = target.reference().getMemoryRequirements();
          uint32_t memoryType = memoryTypeIndexFunction(
              *physicalDevice.lock(),
              memoryRequirements.memoryTypeBits,
              target.memoryCreateInfo
          );
          target.memory = std::make_shared<vk::raii::DeviceMemory>(
              *device.lock(),
              vk::MemoryAllocateInfo()
                  .setAllocationSize(memoryRequirements.size)
                  .setMemoryTypeIndex(memoryType)
          );
          target.reference().bindMemory(*target.memoryReference(), 0);
          return target;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  Image::Builder Image::builder() {
    return {};
  }

}
