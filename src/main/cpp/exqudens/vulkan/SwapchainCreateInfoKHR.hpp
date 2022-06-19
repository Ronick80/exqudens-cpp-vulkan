#pragma once

#include <vector>

#include <vulkan/vulkan_structs.hpp>

namespace exqudens::vulkan {

  struct SwapchainCreateInfoKHR: vk::SwapchainCreateInfoKHR {

    SwapchainCreateInfoKHR& setFlags(const vk::SwapchainCreateFlagsKHR& value) {
      vk::SwapchainCreateInfoKHR::setFlags(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setSurface(const vk::SurfaceKHR& value) {
      vk::SwapchainCreateInfoKHR::setSurface(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setMinImageCount(const uint32_t& value) {
      vk::SwapchainCreateInfoKHR::setMinImageCount(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setImageFormat(const vk::Format& value) {
      vk::SwapchainCreateInfoKHR::setImageFormat(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setImageColorSpace(const vk::ColorSpaceKHR& value) {
      vk::SwapchainCreateInfoKHR::setImageColorSpace(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setImageExtent(const vk::Extent2D& value) {
      vk::SwapchainCreateInfoKHR::setImageExtent(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setImageArrayLayers(const uint32_t& value) {
      vk::SwapchainCreateInfoKHR::setImageArrayLayers(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setImageUsage(const vk::ImageUsageFlags& value) {
      vk::SwapchainCreateInfoKHR::setImageUsage(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setImageSharingMode(const vk::SharingMode& value) {
      vk::SwapchainCreateInfoKHR::setImageSharingMode(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setPreTransform(const vk::SurfaceTransformFlagBitsKHR& value) {
      vk::SwapchainCreateInfoKHR::setPreTransform(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setCompositeAlpha(const vk::CompositeAlphaFlagBitsKHR& value) {
      vk::SwapchainCreateInfoKHR::setCompositeAlpha(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setPresentMode(const vk::PresentModeKHR& value) {
      vk::SwapchainCreateInfoKHR::setPresentMode(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setClipped(const vk::Bool32& value) {
      vk::SwapchainCreateInfoKHR::setClipped(value);
      return *this;
    }

    SwapchainCreateInfoKHR& setOldSwapchain(const vk::SwapchainKHR& value) {
      vk::SwapchainCreateInfoKHR::setOldSwapchain(value);
      return *this;
    }

    std::vector<uint32_t> queueFamilyIndices;

    SwapchainCreateInfoKHR& setQueueFamilyIndices(const std::vector<uint32_t>& value) {
      queueFamilyIndices = value;
      vk::SwapchainCreateInfoKHR::setQueueFamilyIndices(queueFamilyIndices);
      return *this;
    }

  };

}
