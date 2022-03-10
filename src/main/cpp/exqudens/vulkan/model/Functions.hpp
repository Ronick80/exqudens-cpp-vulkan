#pragma once

#include <cstdint>
#include <functional>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Functions {

    std::function<void(
        uint32_t*                                   pPropertyCount,
        VkLayerProperties*                          pProperties
    )> enumerateInstanceLayerProperties = vkEnumerateInstanceLayerProperties;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        uint32_t*                                   pQueueFamilyPropertyCount,
        VkQueueFamilyProperties*                    pQueueFamilyProperties
    )> getPhysicalDeviceQueueFamilyProperties = vkGetPhysicalDeviceQueueFamilyProperties;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        uint32_t                                    queueFamilyIndex,
        VkSurfaceKHR                                surface,
        VkBool32*                                   pSupported
    )> getPhysicalDeviceSurfaceSupportKHR = vkGetPhysicalDeviceSurfaceSupportKHR;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        const char*                                 pLayerName,
        uint32_t*                                   pPropertyCount,
        VkExtensionProperties*                      pProperties
    )> enumerateDeviceExtensionProperties = vkEnumerateDeviceExtensionProperties;

    std::function<void(
        VkInstance                                  instance,
        uint32_t*                                   pPhysicalDeviceCount,
        VkPhysicalDevice*                           pPhysicalDevices
    )> enumeratePhysicalDevices = vkEnumeratePhysicalDevices;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkPhysicalDeviceFeatures*                   pFeatures
    )> getPhysicalDeviceFeatures = vkGetPhysicalDeviceFeatures;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkSurfaceKHR                                surface,
        VkSurfaceCapabilitiesKHR*                   pSurfaceCapabilities
    )> getPhysicalDeviceSurfaceCapabilitiesKHR = vkGetPhysicalDeviceSurfaceCapabilitiesKHR;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkSurfaceKHR                                surface,
        uint32_t*                                   pSurfaceFormatCount,
        VkSurfaceFormatKHR*                         pSurfaceFormats
    )> getPhysicalDeviceSurfaceFormatsKHR = vkGetPhysicalDeviceSurfaceFormatsKHR;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkSurfaceKHR                                surface,
        uint32_t*                                   pPresentModeCount,
        VkPresentModeKHR*                           pPresentModes
    )> getPhysicalDeviceSurfacePresentModesKHR = vkGetPhysicalDeviceSurfacePresentModesKHR;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkPhysicalDeviceMemoryProperties*           pMemoryProperties
    )> getPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkFormat                                    format,
        VkFormatProperties*                         pFormatProperties
    )> getPhysicalDeviceFormatProperties = vkGetPhysicalDeviceFormatProperties;

    std::function<PFN_vkVoidFunction(
        VkInstance                                  instance,
        const char*                                 pName
    )> getInstanceProcAddr = vkGetInstanceProcAddr;

    std::function<void(
        VkDevice                                    device,
        uint32_t                                    queueFamilyIndex,
        uint32_t                                    queueIndex,
        VkQueue*                                    pQueue
    )> getDeviceQueue = vkGetDeviceQueue;

    std::function<void(
        VkDevice                                    device,
        VkBuffer                                    buffer,
        VkMemoryRequirements*                       pMemoryRequirements
    )> getBufferMemoryRequirements = vkGetBufferMemoryRequirements;

    std::function<void(
        VkDevice                                    device,
        VkImage                                     image,
        VkMemoryRequirements*                       pMemoryRequirements
    )> getImageMemoryRequirements = vkGetImageMemoryRequirements;

    std::function<VkResult(
        VkDevice                                    device,
        VkSwapchainKHR                              swapchain,
        uint32_t*                                   pSwapchainImageCount,
        VkImage*                                    pSwapchainImages
    )> getSwapchainImagesKHR = vkGetSwapchainImagesKHR;

    std::function<void(
        VkDevice                                    device,
        uint32_t                                    descriptorWriteCount,
        const VkWriteDescriptorSet*                 pDescriptorWrites,
        uint32_t                                    descriptorCopyCount,
        const VkCopyDescriptorSet*                  pDescriptorCopies
    )> updateDescriptorSets = vkUpdateDescriptorSets;

    std::function<VkResult(
        const VkInstanceCreateInfo*                 pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkInstance*                                 pInstance
    )> createInstance = vkCreateInstance;

    std::function<VkResult(
        VkPhysicalDevice                            physicalDevice,
        const VkDeviceCreateInfo*                   pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkDevice*                                   pDevice
    )> createDevice = vkCreateDevice;

    std::function<VkResult(
        VkDevice                                    device,
        const VkCommandPoolCreateInfo*              pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkCommandPool*                              pCommandPool
    )> createCommandPool = vkCreateCommandPool;

    std::function<VkResult(
        VkDevice                                    device,
        const VkSwapchainCreateInfoKHR*             pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkSwapchainKHR*                             pSwapchain
    )> createSwapchainKHR = vkCreateSwapchainKHR;

    std::function<VkResult(
        VkDevice                                    device,
        const VkBufferCreateInfo*                   pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkBuffer*                                   pBuffer
    )> createBuffer = vkCreateBuffer;

    std::function<VkResult(
        VkDevice                                    device,
        const VkImageCreateInfo*                    pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkImage*                                    pImage
    )> createImage = vkCreateImage;

    std::function<VkResult(
        VkDevice                                    device,
        const VkImageViewCreateInfo*                pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkImageView*                                pView
    )> createImageView = vkCreateImageView;

    std::function<VkResult(
        VkDevice                                    device,
        const VkRenderPassCreateInfo*               pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkRenderPass*                               pRenderPass
    )> createRenderPass = vkCreateRenderPass;

    std::function<VkResult(
        VkDevice                                    device,
        const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkDescriptorSetLayout*                      pSetLayout
    )> createDescriptorSetLayout = vkCreateDescriptorSetLayout;

    std::function<VkResult(
        VkDevice                                    device,
        const VkShaderModuleCreateInfo*             pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkShaderModule*                             pShaderModule
    )> createShaderModule = vkCreateShaderModule;

    std::function<VkResult(
        VkDevice                                    device,
        const VkPipelineLayoutCreateInfo*           pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkPipelineLayout*                           pPipelineLayout
    )> createPipelineLayout = vkCreatePipelineLayout;

    std::function<VkResult(
        VkDevice                                    device,
        VkPipelineCache                             pipelineCache,
        uint32_t                                    createInfoCount,
        const VkGraphicsPipelineCreateInfo*         pCreateInfos,
        const VkAllocationCallbacks*                pAllocator,
        VkPipeline*                                 pPipelines
    )> createGraphicsPipelines = vkCreateGraphicsPipelines;

    std::function<VkResult(
        VkDevice                                    device,
        const VkFramebufferCreateInfo*              pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkFramebuffer*                              pFramebuffer
    )> createFramebuffer = vkCreateFramebuffer;

    std::function<VkResult(
        VkDevice                                    device,
        const VkSamplerCreateInfo*                  pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkSampler*                                  pSampler
    )> createSampler = vkCreateSampler;

    std::function<VkResult(
        VkDevice                                    device,
        const VkDescriptorPoolCreateInfo*           pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkDescriptorPool*                           pDescriptorPool
    )> createDescriptorPool = vkCreateDescriptorPool;

    std::function<VkResult(
        VkDevice                                    device,
        const VkSemaphoreCreateInfo*                pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkSemaphore*                                pSemaphore
    )> createSemaphore = vkCreateSemaphore;

    std::function<VkResult(
        VkDevice                                    device,
        const VkFenceCreateInfo*                    pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkFence*                                    pFence
    )> createFence = vkCreateFence;

    std::function<VkResult(
        VkDevice                                    device,
        const VkMemoryAllocateInfo*                 pAllocateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkDeviceMemory*                             pMemory
    )> allocateMemory = vkAllocateMemory;

    std::function<VkResult(
        VkDevice                                    device,
        const VkDescriptorSetAllocateInfo*          pAllocateInfo,
        VkDescriptorSet*                            pDescriptorSets
    )> allocateDescriptorSets = vkAllocateDescriptorSets;

    std::function<VkResult(
        VkDevice                                    device,
        const VkCommandBufferAllocateInfo*          pAllocateInfo,
        VkCommandBuffer*                            pCommandBuffers
    )> allocateCommandBuffers = vkAllocateCommandBuffers;

    std::function<VkResult(
        VkDevice                                    device,
        VkBuffer                                    buffer,
        VkDeviceMemory                              memory,
        VkDeviceSize                                memoryOffset
    )> bindBufferMemory = vkBindBufferMemory;

    std::function<VkResult(
        VkDevice                                    device,
        VkImage                                     image,
        VkDeviceMemory                              memory,
        VkDeviceSize                                memoryOffset
    )> bindImageMemory = vkBindImageMemory;

    std::function<void(
        VkDevice                                    device,
        VkDeviceMemory                              memory
    )> unmapMemory = vkUnmapMemory;

    std::function<void(
        VkDevice                                    device,
        VkDeviceMemory                              memory,
        const VkAllocationCallbacks*                pAllocator
    )> freeMemory = vkFreeMemory;

    std::function<void(
        VkDevice                                    device,
        VkCommandPool                               commandPool,
        uint32_t                                    commandBufferCount,
        const VkCommandBuffer*                      pCommandBuffers
    )> freeCommandBuffers = vkFreeCommandBuffers;

    std::function<void(
        VkDevice                                    device,
        VkFence                                     fence,
        const VkAllocationCallbacks*                pAllocator
    )> destroyFence = vkDestroyFence;

    std::function<void(
        VkDevice                                    device,
        VkSemaphore                                 semaphore,
        const VkAllocationCallbacks*                pAllocator
    )> destroySemaphore = vkDestroySemaphore;

    std::function<void(
        VkDevice                                    device,
        VkDescriptorPool                            descriptorPool,
        const VkAllocationCallbacks*                pAllocator
    )> destroyDescriptorPool = vkDestroyDescriptorPool;

    std::function<void(
        VkDevice                                    device,
        VkSampler                                   sampler,
        const VkAllocationCallbacks*                pAllocator
    )> destroySampler = vkDestroySampler;

    std::function<void(
        VkDevice                                    device,
        VkFramebuffer                               framebuffer,
        const VkAllocationCallbacks*                pAllocator
    )> destroyFramebuffer = vkDestroyFramebuffer;

    std::function<void(
        VkDevice                                    device,
        VkPipeline                                  pipeline,
        const VkAllocationCallbacks*                pAllocator
    )> destroyPipeline = vkDestroyPipeline;

    std::function<void(
        VkDevice                                    device,
        VkShaderModule                              shaderModule,
        const VkAllocationCallbacks*                pAllocator
    )> destroyShaderModule = vkDestroyShaderModule;

    std::function<void(
        VkDevice                                    device,
        VkDescriptorSetLayout                       descriptorSetLayout,
        const VkAllocationCallbacks*                pAllocator
    )> destroyDescriptorSetLayout = vkDestroyDescriptorSetLayout;

    std::function<void(
        VkDevice                                    device,
        VkRenderPass                                renderPass,
        const VkAllocationCallbacks*                pAllocator
    )> destroyRenderPass = vkDestroyRenderPass;

    std::function<void(
        VkDevice                                    device,
        VkImageView                                 imageView,
        const VkAllocationCallbacks*                pAllocator
    )> destroyImageView = vkDestroyImageView;

    std::function<void(
        VkDevice                                    device,
        VkImage                                     image,
        const VkAllocationCallbacks*                pAllocator
    )> destroyImage = vkDestroyImage;

    std::function<void(
        VkDevice                                    device,
        VkBuffer                                    buffer,
        const VkAllocationCallbacks*                pAllocator
    )> destroyBuffer = vkDestroyBuffer;

    std::function<void(
        VkDevice                                    device,
        VkSwapchainKHR                              swapchain,
        const VkAllocationCallbacks*                pAllocator
    )> destroySwapchainKHR = vkDestroySwapchainKHR;

    std::function<void(
        VkDevice                                    device,
        VkCommandPool                               commandPool,
        const VkAllocationCallbacks*                pAllocator
    )> destroyCommandPool = vkDestroyCommandPool;

    std::function<void(
        VkDevice                                    device,
        const VkAllocationCallbacks*                pAllocator
    )> destroyDevice = vkDestroyDevice;

    std::function<void(
        VkInstance                                  instance,
        VkSurfaceKHR                                surface,
        const VkAllocationCallbacks*                pAllocator
    )> destroySurfaceKHR = vkDestroySurfaceKHR;

    std::function<void(
        VkInstance                                  instance,
        const VkAllocationCallbacks*                pAllocator
    )> destroyInstance = vkDestroyInstance;

  };

}
