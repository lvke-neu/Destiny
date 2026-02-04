#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Destiny
{
    class VulkanBuffer
    {
    public:
        VulkanBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
            : m_device(device), m_size(size)
        {
            createBuffer(physicalDevice, size, usage, properties, m_buffer, m_bufferMemory);
        }

        ~VulkanBuffer()
        {
            if (m_buffer) vkDestroyBuffer(m_device, m_buffer, nullptr);
            if (m_bufferMemory) vkFreeMemory(m_device, m_bufferMemory, nullptr);
        }

        VkBuffer getBuffer() const { return m_buffer; }
        VkDeviceMemory getMemory() const { return m_bufferMemory; }
        VkDeviceSize getSize() const { return m_size; }

        void map(void** data)
        {
            vkMapMemory(m_device, m_bufferMemory, 0, m_size, 0, data);
        }

        void unmap()
        {
            vkUnmapMemory(m_device, m_bufferMemory);
        }

    private:
        void createBuffer(VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
        {
            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usage;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(m_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create buffer!");
            }

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(m_device, buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(m_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to allocate buffer memory!");
            }

            vkBindBufferMemory(m_device, buffer, bufferMemory, 0);
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
        VkBuffer m_buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_bufferMemory = VK_NULL_HANDLE;
        VkDeviceSize m_size;
    };
}
