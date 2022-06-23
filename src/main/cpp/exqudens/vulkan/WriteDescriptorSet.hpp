#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan {

  struct WriteDescriptorSet: vk::WriteDescriptorSet {

    WriteDescriptorSet& setDstSet(const vk::DescriptorSet& value) {
      vk::WriteDescriptorSet::setDstSet(value);
      return *this;
    }

    WriteDescriptorSet& setDstBinding(const uint32_t& value) {
      vk::WriteDescriptorSet::setDstBinding(value);
      return *this;
    }

    WriteDescriptorSet& setDstArrayElement(const uint32_t& value) {
      vk::WriteDescriptorSet::setDstArrayElement(value);
      return *this;
    }

    WriteDescriptorSet& setDescriptorCount(const uint32_t& value) {
      vk::WriteDescriptorSet::setDescriptorCount(value);
      return *this;
    }

    WriteDescriptorSet& setDescriptorType(const vk::DescriptorType& value) {
      vk::WriteDescriptorSet::setDescriptorType(value);
      return *this;
    }

    std::vector<vk::DescriptorImageInfo> imageInfo;
    std::vector<vk::DescriptorBufferInfo> bufferInfo;
    std::vector<vk::BufferView> texelBufferView;

    WriteDescriptorSet& setImageInfo(const std::vector<vk::DescriptorImageInfo>& value) {
      imageInfo = value;
      vk::WriteDescriptorSet::setImageInfo(imageInfo);
      return *this;
    }

    WriteDescriptorSet& setBufferInfo(const std::vector<vk::DescriptorBufferInfo>& value) {
      bufferInfo = value;
      vk::WriteDescriptorSet::setBufferInfo(bufferInfo);
      return *this;
    }

    WriteDescriptorSet& setTexelBufferView(const std::vector<vk::BufferView>& value) {
      texelBufferView = value;
      vk::WriteDescriptorSet::setTexelBufferView(texelBufferView);
      return *this;
    }

  };

}
