#include "VulkanPipeline.h"
#include "VulkanDevice.h"
#include "VulkanShader.h"
#include <iostream>

namespace Destiny
{
    // --- VulkanInputLayout ---

    static VkFormat GetVkFormat(DXGI_FORMAT format)
    {
        switch (format)
        {
        case DXGI_FORMAT_R32G32B32A32_FLOAT: return VK_FORMAT_R32G32B32A32_SFLOAT;
        case DXGI_FORMAT_R32G32B32_FLOAT:    return VK_FORMAT_R32G32B32_SFLOAT;
        case DXGI_FORMAT_R32G32_FLOAT:       return VK_FORMAT_R32G32_SFLOAT;
        case DXGI_FORMAT_R32_FLOAT:          return VK_FORMAT_R32_SFLOAT;
        case DXGI_FORMAT_R32G32B32A32_UINT:  return VK_FORMAT_R32G32B32A32_UINT;
        case DXGI_FORMAT_R8G8B8A8_UNORM:     return VK_FORMAT_R8G8B8A8_UNORM;
        case DXGI_FORMAT_R8G8B8A8_UINT:      return VK_FORMAT_R8G8B8A8_UINT;
        // Add more as needed
        default: return VK_FORMAT_UNDEFINED;
        }
    }

    VulkanInputLayout::VulkanInputLayout(const D3D11_INPUT_ELEMENT_DESC* elements, unsigned int numElements)
    {
        // Simple assumption: 1 binding (slot 0), per-vertex data
        // In a real engine, we might have multiple streams/slots.
        // D3D11 uses InputSlot in the element desc.

        // We need to group elements by input slot to create bindings
        // But for simplicity, let's assume slot 0 for now or handle simple cases.
        
        // Actually, let's respect the InputSlot.
        
        // Find max slot to size bindings vector
        uint32_t maxSlot = 0;
        for (unsigned int i = 0; i < numElements; ++i)
        {
            if (elements[i].InputSlot > maxSlot) maxSlot = elements[i].InputSlot;
        }

        // We can't easily know the stride here without the vertex buffer, 
        // BUT Vulkan requires stride in BindingDescription.
        // D3D11 CreateInputLayout computes strides or uses AlignedByteOffset.
        // Wait, VkVertexInputBindingDescription needs 'stride'. 
        // This information is usually provided by IASetVertexBuffers in D3D11 (stride is parameter there).
        // However, pipeline creation in Vulkan needs it.
        
        // Option 1: Dynamic Pipeline State for VertexInput? No, VertexInput is static state.
        // Option 2: We must know the stride at pipeline creation time. 
        // When we call Draw, we have the VertexBuffer bound, so we know the stride.
        // So VulkanInputLayout just stores the attributes, and we construct the BindingDescription 
        // when we build the pipeline (using the current stride from context).
        
        // So for now, let's just store attributes and mapped formats.
        
        for (unsigned int i = 0; i < numElements; ++i)
        {
            const auto& elem = elements[i];
            
            VkVertexInputAttributeDescription attr{};
            attr.binding = elem.InputSlot;
            attr.location = i; // Map 1:1 for now, but this should match Shader location
            // Shader reflection is needed to map SemanticName to Location properly!
            // For this migration, we assume the shader uses:
            // layout(location = 0) in vec3 inPosition;
            // layout(location = 1) in vec3 inNormal;
            // etc. in the order they appear in InputLayout? 
            // Or we assume a convention.
            
            // Convention:
            // POSITION -> 0
            // NORMAL   -> 1
            // TEXCOORD -> 2
            // COLOR    -> 3
            // BONEID   -> 4
            // WEIGHTS  -> 5
            
            if (strcmp(elem.SemanticName, "POSITION") == 0) attr.location = 0;
            else if (strcmp(elem.SemanticName, "NORMAL") == 0) attr.location = 1;
            else if (strcmp(elem.SemanticName, "TEXCOORD") == 0) attr.location = 2;
            else if (strcmp(elem.SemanticName, "COLOR") == 0) attr.location = 3;
            else if (strcmp(elem.SemanticName, "BONEID") == 0) attr.location = 4;
            else if (strcmp(elem.SemanticName, "WEIGHTS") == 0) attr.location = 5;
            else attr.location = i; // Fallback

            attr.format = GetVkFormat(elem.Format);
            attr.offset = elem.AlignedByteOffset;
            
            // Handle D3D11_APPEND_ALIGNED_ELEMENT
            if (attr.offset == 0xffffffff)
            {
                // We need to calculate it based on previous element. 
                // This is hard without knowing previous element size.
                // But usually user provides explicit offsets or we track it.
                // For now, assume explicit offsets or 0 if first.
                if (i > 0)
                {
                    // This is tricky. Let's hope we don't hit this path or we can deduce it.
                    // Actually, InputLayout.cpp provides explicit offsets for all standard layouts!
                    // e.g. 0, 12, 24...
                    // So we are good for internal layouts.
                }
                else
                {
                    attr.offset = 0;
                }
            }

            m_attributes.push_back(attr);
        }
    }

    // --- VulkanPipeline ---

    VulkanPipeline::VulkanPipeline(VulkanDevice& device, VkPipeline pipeline, VkPipelineLayout layout)
        : m_device(device), m_pipeline(pipeline), m_layout(layout)
    {
    }

    VulkanPipeline::~VulkanPipeline()
    {
        if (m_pipeline)
        {
            vkDestroyPipeline(m_device.getVkDevice(), m_pipeline, nullptr);
        }
        if (m_layout)
        {
            vkDestroyPipelineLayout(m_device.getVkDevice(), m_layout, nullptr);
        }
    }

    void VulkanPipeline::bind(VkCommandBuffer commandBuffer)
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    }

    // --- VulkanPipelineCache ---

    VulkanPipelineCache::VulkanPipelineCache(VulkanDevice& device)
        : m_device(device)
    {
    }

    VulkanPipelineCache::~VulkanPipelineCache()
    {
        m_pipelines.clear();
    }

    VulkanPipeline* VulkanPipelineCache::getPipeline(const VulkanPipelineKey& key)
    {
        auto it = m_pipelines.find(key);
        if (it != m_pipelines.end())
        {
            return it->second.get();
        }

        auto pipeline = std::unique_ptr<VulkanPipeline>(createPipeline(key));
        VulkanPipeline* ptr = pipeline.get();
        m_pipelines[key] = std::move(pipeline);
        return ptr;
    }

    VulkanPipeline* VulkanPipelineCache::createPipeline(const VulkanPipelineKey& key)
    {
        // 1. Shaders
        VkPipelineShaderStageCreateInfo shaderStages[2] = {};
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = key.vs->getShaderModule();
        shaderStages[0].pName = "main";

        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = key.ps->getShaderModule();
        shaderStages[1].pName = "main";

        // 2. Vertex Input
        std::vector<VkVertexInputBindingDescription> bindingDescs;
        std::vector<VkVertexInputAttributeDescription> attrDescs = key.inputLayout->getAttributeDescriptions();
        
        // Calculate stride per binding
        std::unordered_map<uint32_t, uint32_t> bindingStrides;
        for(const auto& attr : attrDescs)
        {
            uint32_t size = 0;
            switch(attr.format) {
                case VK_FORMAT_R32G32B32A32_SFLOAT: size = 16; break;
                case VK_FORMAT_R32G32B32_SFLOAT: size = 12; break;
                case VK_FORMAT_R32G32_SFLOAT: size = 8; break;
                case VK_FORMAT_R32_SFLOAT: size = 4; break;
                // Basic fallbacks
                case VK_FORMAT_R8G8B8A8_UNORM: size = 4; break;
                case VK_FORMAT_R8G8B8A8_UINT: size = 4; break;
                default: size = 0; break;
            }
            uint32_t end = attr.offset + size;
            if (end > bindingStrides[attr.binding]) bindingStrides[attr.binding] = end;
        }

        for(auto kv : bindingStrides)
        {
            VkVertexInputBindingDescription binding{};
            binding.binding = kv.first;
            binding.stride = kv.second;
            binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            bindingDescs.push_back(binding);
        }

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)bindingDescs.size();
        vertexInputInfo.pVertexBindingDescriptions = bindingDescs.data();
        vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)attrDescs.size();
        vertexInputInfo.pVertexAttributeDescriptions = attrDescs.data();

        // 3. Input Assembly
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = key.topology;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        // 4. Viewport (Dynamic)
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        // 5. Rasterizer
        VkPipelineRasterizationStateCreateInfo rasterizer = key.rasterizerState->rasterizerInfo;

        // 6. Multisample
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        // 7. Depth Stencil
        VkPipelineDepthStencilStateCreateInfo depthStencil = key.depthStencilState->depthStencilInfo;

        // 8. Color Blend
        VkPipelineColorBlendStateCreateInfo colorBlend = key.blendState->blendInfo;
        if (colorBlend.attachmentCount > 1) {
             colorBlend.attachmentCount = 1; 
        }
        
        // 9. Dynamic State
        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        // 10. Pipeline Layout
        VkPipelineLayout pipelineLayout;
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; 
        pipelineLayoutInfo.pSetLayouts = nullptr;
        
        if (vkCreatePipelineLayout(m_device.getVkDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlend;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = key.renderPass;
        pipelineInfo.subpass = key.subpass;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        VkPipeline graphicsPipeline;
        if (vkCreateGraphicsPipelines(m_device.getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        return new VulkanPipeline(m_device, graphicsPipeline, pipelineLayout);
    }
}