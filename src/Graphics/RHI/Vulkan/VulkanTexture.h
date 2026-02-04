#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>

namespace Destiny
{
    class VulkanTexture
    {
    public:
        VulkanTexture(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags)
            : m_device(device), m_width(width), m_height(height), m_format(format), m_ownsImage(true)
        {
            createImage(physicalDevice, width, height, format, tiling, usage, properties, m_image, m_imageMemory);
            m_imageView = createImageView(m_image, format, aspectFlags);
        }

        VulkanTexture(VkDevice device, VkImage image, VkImageView view, uint32_t width, uint32_t height, VkFormat format)
            : m_device(device), m_image(image), m_imageView(view), m_width(width), m_height(height), m_format(format), m_ownsImage(false)
        {
        }

        ~VulkanTexture()
        {
            if (m_ownsImage)
            {
                if (m_imageView) vkDestroyImageView(m_device, m_imageView, nullptr);
                if (m_image) vkDestroyImage(m_device, m_image, nullptr);
                if (m_imageMemory) vkFreeMemory(m_device, m_imageMemory, nullptr);
            }
        }

        VkImage getImage() const { return m_image; }
        VkImageView getImageView() const { return m_imageView; }
        VkFormat getFormat() const { return m_format; }
        uint32_t getWidth() const { return m_width; }
        uint32_t getHeight() const { return m_height; }
        VkImageLayout getLayout() const { return m_layout; }

        void setLayout(VkImageLayout layout) { m_layout = layout; }

    private:
        void createImage(VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
        {
            VkImageCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = width;
            imageInfo.extent.height = height;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = 1;
            imageInfo.format = format;
            imageInfo.tiling = tiling;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = usage;
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateImage(m_device, &imageInfo, nullptr, &image) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image!");
            }

            VkMemoryRequirements memRequirements;
            vkGetImageMemoryRequirements(m_device, image, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(m_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to allocate image memory!");
            }

            vkBindImageMemory(m_device, image, imageMemory, 0);
        }

        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
        {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = format;
            viewInfo.subresourceRange.aspectMask = aspectFlags;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            VkImageView imageView;
            if (vkCreateImageView(m_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create texture image view!");
            }

            return imageView;
        }

        uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
        {
            VkPhysicalDeviceMemoryProperties memProperties;
            vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

            for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
            {
                if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                {
                    return i;
                }
            }

            throw std::runtime_error("failed to find suitable memory type!");
        }

        VkDevice m_device;
        VkImage m_image = VK_NULL_HANDLE;
        VkDeviceMemory m_imageMemory = VK_NULL_HANDLE;
        VkImageView m_imageView = VK_NULL_HANDLE;
        uint32_t m_width, m_height;
        VkFormat m_format;
        VkImageLayout m_layout = VK_IMAGE_LAYOUT_UNDEFINED;
        bool m_ownsImage;
    };
}
