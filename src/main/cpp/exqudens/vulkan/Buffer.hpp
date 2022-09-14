#pragma once

#include <optional>
#include <memory>
#include <stdexcept>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan {

  struct Buffer {

    class Builder;

    inline static Builder builder();

    vk::BufferCreateInfo createInfo;
    std::shared_ptr<vk::raii::Buffer> value;
    vk::MemoryPropertyFlags memoryCreateInfo;
    std::shared_ptr<vk::raii::DeviceMemory> memory;

    vk::raii::Buffer& reference() {
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

  class Buffer::Builder {

    private:

      std::weak_ptr<vk::raii::PhysicalDevice> physicalDevice;
      std::weak_ptr<vk::raii::Device> device;
      std::function<uint32_t(
          vk::raii::PhysicalDevice&,
          const uint32_t&,
          const vk::MemoryPropertyFlags&
      )> memoryTypeIndexFunction;
      std::optional<vk::BufferCreateInfo> createInfo;
      std::optional<vk::MemoryPropertyFlags> memoryCreateInfo;

    public:

      Buffer::Builder& setPhysicalDevice(const std::weak_ptr<vk::raii::PhysicalDevice>& val) {
        physicalDevice = val;
        return *this;
      }

      Buffer::Builder& setDevice(const std::weak_ptr<vk::raii::Device>& val) {
        device = val;
        return *this;
      }

      Buffer::Builder& setMemoryTypeIndexFunction(
          const std::function<uint32_t(
              vk::raii::PhysicalDevice&,
              const uint32_t&,
              const vk::MemoryPropertyFlags&
          )>& val
      ) {
        memoryTypeIndexFunction = val;
        return *this;
      }

      Buffer::Builder& setCreateInfo(const vk::BufferCreateInfo& val) {
        createInfo = val;
        return *this;
      }

      Buffer::Builder& setMemoryCreateInfo(const vk::MemoryPropertyFlags& val) {
        memoryCreateInfo = val;
        return *this;
      }

      Buffer build() {
        try {
          if (!memoryTypeIndexFunction) {
            memoryTypeIndexFunction = &Utility::memoryTypeIndex;
          }

          Buffer target = {};
          target.createInfo = createInfo.value();
          target.value = std::make_shared<vk::raii::Buffer>(
              *device.lock(),
              target.createInfo
          );
          vk::MemoryRequirements memoryRequirements = target.reference().getMemoryRequirements();
          uint32_t memoryType = memoryTypeIndexFunction(
              *physicalDevice.lock(),
              memoryRequirements.memoryTypeBits,
              target.memoryCreateInfo
          );
          target.memoryCreateInfo = memoryCreateInfo.value();
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

  Buffer::Builder Buffer::builder() {
    return {};
  }

}
