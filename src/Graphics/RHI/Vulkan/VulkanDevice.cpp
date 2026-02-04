#include "VulkanDevice.h"
#include "VulkanContext.h"
#include "VulkanSwapChain.h"
#include "VulkanShader.h"
#include "VulkanBuffer.h"
#include "VulkanTexture.h"
#include "VulkanPipeline.h"
#include <iostream>
#include <set>
#include <stdexcept>
#include <d3d11.h> // For D3D11_BIND_FLAG
#include "Engine/Utility.h"

namespace Destiny
{
    VulkanDevice::VulkanDevice()
    {
    }

    VulkanDevice::~VulkanDevice()
    {
        m_immediateContext.reset(); // Destroy context first

        if (m_device)
        {
            if (m_vsDescriptorSetLayout != VK_NULL_HANDLE)
            {
                vkDestroyDescriptorSetLayout(m_device, m_vsDescriptorSetLayout, nullptr);
            }
            if (m_psDescriptorSetLayout != VK_NULL_HANDLE)
            {
                vkDestroyDescriptorSetLayout(m_device, m_psDescriptorSetLayout, nullptr);
            }
            vkDestroyDevice(m_device, nullptr);
        }
        if (m_instance)
        {
            vkDestroyInstance(m_instance, nullptr);
        }
    }

    void VulkanDevice::initialize(void* windowHandle)
    {
        createInstance();
        pickPhysicalDevice();
        createLogicalDevice();
        createDescriptorSetLayouts();  // Create standard layouts before pipelines
        m_pipelineCache = std::make_unique<VulkanPipelineCache>(*this);
        m_renderPassCache = std::make_unique<VulkanRenderPassCache>(*this);
        m_framebufferCache = std::make_unique<VulkanFramebufferCache>(*this);
        m_immediateContext = std::make_shared<VulkanContext>(this);
    }

    std::shared_ptr<GraphicsContext> VulkanDevice::getImmediateContext()
    {
        return m_immediateContext;
    }

    std::shared_ptr<SwapChain> VulkanDevice::createSwapChain(void* windowHandle, int width, int height)
    {
        return std::make_shared<VulkanSwapChain>(this, windowHandle, width, height);
    }

    long VulkanDevice::CreateVertexShader(const void* shaderBytecode, size_t bytecodeLength, void* classLinkage, void** ppVertexShader)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = bytecodeLength;
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderBytecode);

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            return -1; // Fail
        }

        *ppVertexShader = new VulkanVertexShader(m_device, shaderModule);
        return 0; // Success
    }

    long VulkanDevice::CreatePixelShader(const void* shaderBytecode, size_t bytecodeLength, void* classLinkage, void** ppPixelShader)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = bytecodeLength;
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderBytecode);

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            return -1; // Fail
        }

        *ppPixelShader = new VulkanPixelShader(m_device, shaderModule);
        return 0; // Success
    }

    long VulkanDevice::CreateBuffer(const void* desc, const void* data, void** ppBuffer)
    {
        const D3D11_BUFFER_DESC* pDesc = (const D3D11_BUFFER_DESC*)desc;
        const D3D11_SUBRESOURCE_DATA* pData = (const D3D11_SUBRESOURCE_DATA*)data;

        VkBufferUsageFlags usage = 0;
        if (pDesc->BindFlags & D3D11_BIND_VERTEX_BUFFER) usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        if (pDesc->BindFlags & D3D11_BIND_INDEX_BUFFER) usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        if (pDesc->BindFlags & D3D11_BIND_CONSTANT_BUFFER) usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        if (pDesc->BindFlags & D3D11_BIND_SHADER_RESOURCE) usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT; // Mapping StructuredBuffer to StorageBuffer

        // For simplicity, we use Host Visible memory for everything initially to allow easy mapping/uploading
        // Optimization: Use DeviceLocal for static buffers and StagingBuffers for upload
        VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        try
        {
            VulkanBuffer* buffer = new VulkanBuffer(m_device, m_physicalDevice, pDesc->ByteWidth, usage, properties);
            
            if (pData && pData->pSysMem)
            {
                void* mappedData;
                buffer->map(&mappedData);
                memcpy(mappedData, pData->pSysMem, pDesc->ByteWidth);
                buffer->unmap();
            }

            *ppBuffer = buffer;
            return 0; // Success
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("Vulkan CreateBuffer failed: " + std::string(e.what()));
            return -1;
        }
    }

    void VulkanDevice::DestroyBuffer(void* buffer)
    {
        if (buffer)
        {
            delete static_cast<VulkanBuffer*>(buffer);
        }
    }

    long VulkanDevice::CreateTexture2D(const void* desc, const void* initialData, void** ppTexture2D)
    {
        const D3D11_TEXTURE2D_DESC* pDesc = (const D3D11_TEXTURE2D_DESC*)desc;
        const D3D11_SUBRESOURCE_DATA* pData = (const D3D11_SUBRESOURCE_DATA*)initialData;

        // Map Format
        VkFormat format = VK_FORMAT_R8G8B8A8_UNORM; // Default/Fallback
        if (pDesc->Format == DXGI_FORMAT_R8G8B8A8_UNORM) format = VK_FORMAT_R8G8B8A8_UNORM;
        else if (pDesc->Format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB) format = VK_FORMAT_R8G8B8A8_SRGB;
        else if (pDesc->Format == DXGI_FORMAT_B8G8R8A8_UNORM) format = VK_FORMAT_B8G8R8A8_UNORM;
        // Add more formats as needed

        VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT; // Needed for upload
        if (pDesc->BindFlags & D3D11_BIND_SHADER_RESOURCE) usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
        if (pDesc->BindFlags & D3D11_BIND_RENDER_TARGET) usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        if (pDesc->BindFlags & D3D11_BIND_DEPTH_STENCIL) usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
        if (pDesc->BindFlags & D3D11_BIND_DEPTH_STENCIL)
        {
            aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
        }

        try
        {
            VulkanTexture* texture = new VulkanTexture(m_device, m_physicalDevice, pDesc->Width, pDesc->Height, format, VK_IMAGE_TILING_OPTIMAL, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, aspectFlags);

            if (pData && pData->pSysMem)
            {
                VkDeviceSize imageSize = pDesc->Width * pDesc->Height * 4; // Assuming 4 bytes per pixel

                VulkanBuffer stagingBuffer(m_device, m_physicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

                void* data;
                stagingBuffer.map(&data);
                memcpy(data, pData->pSysMem, static_cast<size_t>(imageSize));
                stagingBuffer.unmap();

                transitionImageLayout(texture->getImage(), format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, aspectFlags);
                copyBufferToImage(stagingBuffer.getBuffer(), texture->getImage(), pDesc->Width, pDesc->Height);
                transitionImageLayout(texture->getImage(), format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, aspectFlags);

                texture->setLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            }
            else
            {
                  // Even if no data, we might want to transition it to a usable state or keep it undefined until use
                  // transitionImageLayout(texture->getImage(), format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, aspectFlags);
                  // texture->setLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            }

            *ppTexture2D = texture;
            return 0;
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("Vulkan CreateTexture2D failed: " + std::string(e.what()));
            return -1;
        }
    }

    void VulkanDevice::DestroyTexture(void* texture)
    {
        if (texture)
        {
            delete static_cast<VulkanTexture*>(texture);
        }
    }

    long VulkanDevice::CreateShaderResourceView(void* resource, const void* desc, void** ppSRView)
    {
        // For now, we assume the resource is a VulkanTexture and we just return its internal ImageView
        // In a full implementation, we should check the desc (D3D11_SHADER_RESOURCE_VIEW_DESC)
        // and create a new VkImageView if necessary (e.g. for different mip levels or formats).
        
        if (!resource) return -1;

        // How do we know if it is a Buffer or Texture?
        // We might need RTTI or a base class. 
        // But based on current usage, it's likely a Texture if called from Create2DSRV path.
        // If it's a Buffer, we haven't implemented VkBufferView yet.
        
        // DANGEROUS CAST assumption: resource is VulkanTexture*
        // TODO: Safe cast or type check
        VulkanTexture* texture = static_cast<VulkanTexture*>(resource);
        *ppSRView = (void*)texture->getImageView();
        return 0;
    }

    long VulkanDevice::CreateRenderTargetView(void* resource, const void* desc, void** ppRTView)
    {
        if (!resource) return -1;
        VulkanTexture* texture = static_cast<VulkanTexture*>(resource);
        
        VulkanImageView* view = new VulkanImageView();
        view->view = texture->getImageView();
        view->format = texture->getFormat();
        view->width = texture->getWidth();
        view->height = texture->getHeight();
        
        *ppRTView = view;
        return 0;
    }

    long VulkanDevice::CreateDepthStencilView(void* resource, const void* desc, void** ppDepthStencilView)
    {
        if (!resource) return -1;
        VulkanTexture* texture = static_cast<VulkanTexture*>(resource);

        VulkanImageView* view = new VulkanImageView();
        view->view = texture->getImageView();
        view->format = texture->getFormat();
        view->width = texture->getWidth();
        view->height = texture->getHeight();
        
        *ppDepthStencilView = view;
        return 0;
    }

    long VulkanDevice::CreateInputLayout(const void* inputElementDescs, unsigned int numElements, const void* shaderBytecode, size_t shaderBytecodeLength, void** ppInputLayout)
    {
        const D3D11_INPUT_ELEMENT_DESC* pElements = (const D3D11_INPUT_ELEMENT_DESC*)inputElementDescs;
        VulkanInputLayout* layout = new VulkanInputLayout(pElements, numElements);
        *ppInputLayout = layout;
        return 0;
    }

    long VulkanDevice::CreateSamplerState(const void* samplerDesc, void** ppSamplerState)
    {
        const D3D11_SAMPLER_DESC* pDesc = (const D3D11_SAMPLER_DESC*)samplerDesc;
        
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        
        // Map Filter
        if (pDesc->Filter == D3D11_FILTER_MIN_MAG_MIP_LINEAR)
        {
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        }
        else if (pDesc->Filter == D3D11_FILTER_MIN_MAG_MIP_POINT)
        {
            samplerInfo.magFilter = VK_FILTER_NEAREST;
            samplerInfo.minFilter = VK_FILTER_NEAREST;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        }
        else
        {
             // Default to Linear
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        }

        // Map Address Mode
        auto mapAddressMode = [](D3D11_TEXTURE_ADDRESS_MODE mode) -> VkSamplerAddressMode {
            switch (mode)
            {
            case D3D11_TEXTURE_ADDRESS_WRAP: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            case D3D11_TEXTURE_ADDRESS_MIRROR: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            case D3D11_TEXTURE_ADDRESS_CLAMP: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            case D3D11_TEXTURE_ADDRESS_BORDER: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            default: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            }
        };

        samplerInfo.addressModeU = mapAddressMode(pDesc->AddressU);
        samplerInfo.addressModeV = mapAddressMode(pDesc->AddressV);
        samplerInfo.addressModeW = mapAddressMode(pDesc->AddressW);
        samplerInfo.anisotropyEnable = VK_FALSE; // pDesc->Filter contains anisotropic flag check needed
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f; // Static for now

        VkSampler sampler;
        if (vkCreateSampler(m_device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
        {
            LOG_ERROR("Failed to create texture sampler!");
            return -1;
        }

        *ppSamplerState = (void*)sampler;
        return 0;
    }

    void VulkanDevice::DestroySamplerState(void* samplerState)
    {
        if (samplerState)
        {
            vkDestroySampler(m_device, (VkSampler)samplerState, nullptr);
        }
    }

    long VulkanDevice::CreateRasterizerState(const void* rasterizerDesc, void** ppRasterizerState)
    {
        const D3D11_RASTERIZER_DESC* pDesc = (const D3D11_RASTERIZER_DESC*)rasterizerDesc;
        VulkanRasterizerState* state = new VulkanRasterizerState();
        
        state->rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        state->rasterizerInfo.depthClampEnable = !pDesc->DepthClipEnable;
        state->rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
        
        state->rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
        if (pDesc->FillMode == D3D11_FILL_WIREFRAME) state->rasterizerInfo.polygonMode = VK_POLYGON_MODE_LINE;
        
        state->rasterizerInfo.cullMode = VK_CULL_MODE_NONE;
        if (pDesc->CullMode == D3D11_CULL_FRONT) state->rasterizerInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
        else if (pDesc->CullMode == D3D11_CULL_BACK) state->rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        
        state->rasterizerInfo.frontFace = pDesc->FrontCounterClockwise ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
        
        state->rasterizerInfo.depthBiasEnable = (pDesc->DepthBias != 0 || pDesc->SlopeScaledDepthBias != 0.0f);
        state->rasterizerInfo.depthBiasConstantFactor = static_cast<float>(pDesc->DepthBias);
        state->rasterizerInfo.depthBiasClamp = pDesc->DepthBiasClamp;
        state->rasterizerInfo.depthBiasSlopeFactor = pDesc->SlopeScaledDepthBias;
        
        state->rasterizerInfo.lineWidth = 1.0f;
        
        *ppRasterizerState = state;
        return 0;
    }

    long VulkanDevice::CreateDepthStencilState(const void* depthStencilDesc, void** ppDepthStencilState)
    {
        const D3D11_DEPTH_STENCIL_DESC* pDesc = (const D3D11_DEPTH_STENCIL_DESC*)depthStencilDesc;
        VulkanDepthStencilState* state = new VulkanDepthStencilState();
        
        state->depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        state->depthStencilInfo.depthTestEnable = pDesc->DepthEnable;
        state->depthStencilInfo.depthWriteEnable = (pDesc->DepthWriteMask == D3D11_DEPTH_WRITE_MASK_ALL);
        
        auto mapCompareOp = [](D3D11_COMPARISON_FUNC func) {
            switch(func) {
                case D3D11_COMPARISON_NEVER: return VK_COMPARE_OP_NEVER;
                case D3D11_COMPARISON_LESS: return VK_COMPARE_OP_LESS;
                case D3D11_COMPARISON_EQUAL: return VK_COMPARE_OP_EQUAL;
                case D3D11_COMPARISON_LESS_EQUAL: return VK_COMPARE_OP_LESS_OR_EQUAL;
                case D3D11_COMPARISON_GREATER: return VK_COMPARE_OP_GREATER;
                case D3D11_COMPARISON_NOT_EQUAL: return VK_COMPARE_OP_NOT_EQUAL;
                case D3D11_COMPARISON_GREATER_EQUAL: return VK_COMPARE_OP_GREATER_OR_EQUAL;
                case D3D11_COMPARISON_ALWAYS: return VK_COMPARE_OP_ALWAYS;
                default: return VK_COMPARE_OP_LESS;
            }
        };
        
        state->depthStencilInfo.depthCompareOp = mapCompareOp(pDesc->DepthFunc);
        state->depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        state->depthStencilInfo.minDepthBounds = 0.0f;
        state->depthStencilInfo.maxDepthBounds = 1.0f;
        
        state->depthStencilInfo.stencilTestEnable = pDesc->StencilEnable;
        
        auto mapStencilOp = [](D3D11_STENCIL_OP op) {
            switch(op) {
                case D3D11_STENCIL_OP_KEEP: return VK_STENCIL_OP_KEEP;
                case D3D11_STENCIL_OP_ZERO: return VK_STENCIL_OP_ZERO;
                case D3D11_STENCIL_OP_REPLACE: return VK_STENCIL_OP_REPLACE;
                case D3D11_STENCIL_OP_INCR_SAT: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
                case D3D11_STENCIL_OP_DECR_SAT: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
                case D3D11_STENCIL_OP_INVERT: return VK_STENCIL_OP_INVERT;
                case D3D11_STENCIL_OP_INCR: return VK_STENCIL_OP_INCREMENT_AND_WRAP;
                case D3D11_STENCIL_OP_DECR: return VK_STENCIL_OP_DECREMENT_AND_WRAP;
                default: return VK_STENCIL_OP_KEEP;
            }
        };

        state->depthStencilInfo.front.failOp = mapStencilOp(pDesc->FrontFace.StencilFailOp);
        state->depthStencilInfo.front.passOp = mapStencilOp(pDesc->FrontFace.StencilPassOp);
        state->depthStencilInfo.front.depthFailOp = mapStencilOp(pDesc->FrontFace.StencilDepthFailOp);
        state->depthStencilInfo.front.compareOp = mapCompareOp(pDesc->FrontFace.StencilFunc);
        state->depthStencilInfo.front.compareMask = pDesc->StencilReadMask;
        state->depthStencilInfo.front.writeMask = pDesc->StencilWriteMask;
        state->depthStencilInfo.front.reference = 0; 

        state->depthStencilInfo.back.failOp = mapStencilOp(pDesc->BackFace.StencilFailOp);
        state->depthStencilInfo.back.passOp = mapStencilOp(pDesc->BackFace.StencilPassOp);
        state->depthStencilInfo.back.depthFailOp = mapStencilOp(pDesc->BackFace.StencilDepthFailOp);
        state->depthStencilInfo.back.compareOp = mapCompareOp(pDesc->BackFace.StencilFunc);
        state->depthStencilInfo.back.compareMask = pDesc->StencilReadMask;
        state->depthStencilInfo.back.writeMask = pDesc->StencilWriteMask;
        state->depthStencilInfo.back.reference = 0;
        
        *ppDepthStencilState = state;
        return 0;
    }

    long VulkanDevice::CreateBlendState(const void* blendStateDesc, void** ppBlendState)
    {
        const D3D11_BLEND_DESC* pDesc = (const D3D11_BLEND_DESC*)blendStateDesc;
        VulkanBlendState* state = new VulkanBlendState();
        
        state->blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        state->blendInfo.logicOpEnable = VK_FALSE;
        state->blendInfo.logicOp = VK_LOGIC_OP_COPY;
        
        auto mapBlendFactor = [](D3D11_BLEND blend) {
            switch(blend) {
                case D3D11_BLEND_ZERO: return VK_BLEND_FACTOR_ZERO;
                case D3D11_BLEND_ONE: return VK_BLEND_FACTOR_ONE;
                case D3D11_BLEND_SRC_COLOR: return VK_BLEND_FACTOR_SRC_COLOR;
                case D3D11_BLEND_INV_SRC_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
                case D3D11_BLEND_SRC_ALPHA: return VK_BLEND_FACTOR_SRC_ALPHA;
                case D3D11_BLEND_INV_SRC_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                case D3D11_BLEND_DEST_ALPHA: return VK_BLEND_FACTOR_DST_ALPHA;
                case D3D11_BLEND_INV_DEST_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
                case D3D11_BLEND_DEST_COLOR: return VK_BLEND_FACTOR_DST_COLOR;
                case D3D11_BLEND_INV_DEST_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
                case D3D11_BLEND_SRC_ALPHA_SAT: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
                case D3D11_BLEND_BLEND_FACTOR: return VK_BLEND_FACTOR_CONSTANT_COLOR;
                case D3D11_BLEND_INV_BLEND_FACTOR: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
                default: return VK_BLEND_FACTOR_ONE;
            }
        };

        auto mapBlendOp = [](D3D11_BLEND_OP op) {
            switch(op) {
                case D3D11_BLEND_OP_ADD: return VK_BLEND_OP_ADD;
                case D3D11_BLEND_OP_SUBTRACT: return VK_BLEND_OP_SUBTRACT;
                case D3D11_BLEND_OP_REV_SUBTRACT: return VK_BLEND_OP_REVERSE_SUBTRACT;
                case D3D11_BLEND_OP_MIN: return VK_BLEND_OP_MIN;
                case D3D11_BLEND_OP_MAX: return VK_BLEND_OP_MAX;
                default: return VK_BLEND_OP_ADD;
            }
        };

        for(int i=0; i<8; ++i)
        {
            const auto& rt = pDesc->IndependentBlendEnable ? pDesc->RenderTarget[i] : pDesc->RenderTarget[0];
            
            VkPipelineColorBlendAttachmentState attach{};
            attach.blendEnable = rt.BlendEnable;
            attach.srcColorBlendFactor = mapBlendFactor(rt.SrcBlend);
            attach.dstColorBlendFactor = mapBlendFactor(rt.DestBlend);
            attach.colorBlendOp = mapBlendOp(rt.BlendOp);
            attach.srcAlphaBlendFactor = mapBlendFactor(rt.SrcBlendAlpha);
            attach.dstAlphaBlendFactor = mapBlendFactor(rt.DestBlendAlpha);
            attach.alphaBlendOp = mapBlendOp(rt.BlendOpAlpha);
            
            attach.colorWriteMask = 0;
            if(rt.RenderTargetWriteMask & D3D11_COLOR_WRITE_ENABLE_RED) attach.colorWriteMask |= VK_COLOR_COMPONENT_R_BIT;
            if(rt.RenderTargetWriteMask & D3D11_COLOR_WRITE_ENABLE_GREEN) attach.colorWriteMask |= VK_COLOR_COMPONENT_G_BIT;
            if(rt.RenderTargetWriteMask & D3D11_COLOR_WRITE_ENABLE_BLUE) attach.colorWriteMask |= VK_COLOR_COMPONENT_B_BIT;
            if(rt.RenderTargetWriteMask & D3D11_COLOR_WRITE_ENABLE_ALPHA) attach.colorWriteMask |= VK_COLOR_COMPONENT_A_BIT;
            
            state->attachments.push_back(attach);
        }
        
        state->blendInfo.attachmentCount = 8;
        state->blendInfo.pAttachments = state->attachments.data();
        
        state->blendInfo.blendConstants[0] = 0.0f;
        state->blendInfo.blendConstants[1] = 0.0f;
        state->blendInfo.blendConstants[2] = 0.0f;
        state->blendInfo.blendConstants[3] = 0.0f;
        
        *ppBlendState = state;
        return 0;
    }

    void VulkanDevice::DestroyRasterizerState(void* state)
    {
        if(state) delete static_cast<VulkanRasterizerState*>(state);
    }

    void VulkanDevice::DestroyDepthStencilState(void* state)
    {
        if(state) delete static_cast<VulkanDepthStencilState*>(state);
    }

    void VulkanDevice::DestroyBlendState(void* state)
    {
        if(state) delete static_cast<VulkanBlendState*>(state);
    }

    void VulkanDevice::createInstance()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Destiny Engine";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Destiny";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        std::vector<const char*> extensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        // Enable validation layers for debugging
#ifdef _DEBUG
        std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
#else
        createInfo.enabledLayerCount = 0;
#endif

        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void VulkanDevice::pickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
        if (deviceCount == 0)
        {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
        m_physicalDevice = devices[0]; // Just pick the first one for now
    }

    void VulkanDevice::createLogicalDevice()
    {
        // Find queue family
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_graphicsQueueFamilyIndex = i;
                break;
            }
            i++;
        }

        float queuePriority = 1.0f;
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;

        std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(m_device, m_graphicsQueueFamilyIndex, 0, &m_graphicsQueue);
    }

    VkCommandBuffer VulkanDevice::beginSingleTimeCommands()
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = static_cast<VulkanContext*>(m_immediateContext.get())->getCommandPool();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void VulkanDevice::endSingleTimeCommands(VkCommandBuffer commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_graphicsQueue);

        vkFreeCommandBuffers(m_device, static_cast<VulkanContext*>(m_immediateContext.get())->getCommandPool(), 1, &commandBuffer);
    }

    void VulkanDevice::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask)
    {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = aspectMask;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else
        {
            // Fallback or warning
            // For now, just do the transition with generic masks if not matched (or throw)
             throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        endSingleTimeCommands(commandBuffer);
    }

    void VulkanDevice::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = { width, height, 1 };

        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        endSingleTimeCommands(commandBuffer);
    }
}
