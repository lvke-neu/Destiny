#include "Application.h"
#include "ViewPortPanel.h"
#include "ScenePanel.h"
#include "PropertyPanel.h"
#include "AssetPanel.h"
#include "DebugDeferredRenderPanel.h"
#include "DebugShadowMapPanel.h"
#include "GraphicsStatPanel.h"
#include "AiPanel.h"
#include "FileDialog.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/EventSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/RenderSystem.h"
// #include "Graphics/VisualScene.h"
// #include "Graphics/Texture.h"
#include "Scene/SceneManager.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "Graphics/RHI/Vulkan/VulkanDevice.h"
#include "Graphics/RHI/Vulkan/VulkanSwapChain.h"
#include "Graphics/RHI/Vulkan/VulkanContext.h"
#include <vulkan/vulkan.h>
#include <d3d11.h>
#include <iostream>
#include "Engine/Utility.h"

static VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;

static void check_vk_result(VkResult err)
{
    if (err == 0) return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0) abort();
}

static const uint32_t g_TriangleVertShader[] = 
{
    0x07230203, 0x00010000, 0x000d000a, 0x00000036, 0x00000000, 0x00020011, 0x00000001,       
    0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e,       
    0x00000000, 0x00000001, 0x0008000f, 0x00000000, 0x00000004, 0x6e69616d, 0x00000000,       
    0x00000022, 0x00000026, 0x00000031, 0x00030003, 0x00000002, 0x000001c2, 0x000a0004,       
    0x475f4c47, 0x4c474f4f, 0x70635f45, 0x74735f70, 0x5f656c79, 0x656e696c, 0x7269645f,       
    0x69746365, 0x00006576, 0x00080004, 0x475f4c47, 0x4c474f4f, 0x6e695f45, 0x64756c63,       
    0x69645f65, 0x74636572, 0x00657669, 0x00040005, 0x00000004, 0x6e69616d, 0x00000000,       
    0x00050005, 0x0000000c, 0x69736f70, 0x6e6f6974, 0x00000073, 0x00040005, 0x00000017,       
    0x6f6c6f63, 0x00007372, 0x00060005, 0x00000020, 0x505f6c67, 0x65567265, 0x78657472,       
    0x00000000, 0x00060006, 0x00000020, 0x00000000, 0x505f6c67, 0x7469736f, 0x006e6f69,       
    0x00070006, 0x00000020, 0x00000001, 0x505f6c67, 0x746e696f, 0x657a6953, 0x00000000,       
    0x00070006, 0x00000020, 0x00000002, 0x435f6c67, 0x4470696c, 0x61747369, 0x0065636e,       
    0x00070006, 0x00000020, 0x00000003, 0x435f6c67, 0x446c6c75, 0x61747369, 0x0065636e,       
    0x00030005, 0x00000022, 0x00000000, 0x00060005, 0x00000026, 0x565f6c67, 0x65747265,       
    0x646e4978, 0x00007865, 0x00050005, 0x00000031, 0x67617266, 0x6f6c6f43, 0x00000072,       
    0x00050048, 0x00000020, 0x00000000, 0x0000000b, 0x00000000, 0x00050048, 0x00000020,       
    0x00000001, 0x0000000b, 0x00000001, 0x00050048, 0x00000020, 0x00000002, 0x0000000b,       
    0x00000003, 0x00050048, 0x00000020, 0x00000003, 0x0000000b, 0x00000004, 0x00030047,       
    0x00000020, 0x00000002, 0x00040047, 0x00000026, 0x0000000b, 0x0000002a, 0x00040047,       
    0x00000031, 0x0000001e, 0x00000000, 0x00020013, 0x00000002, 0x00030021, 0x00000003,       
    0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006,       
    0x00000002, 0x00040015, 0x00000008, 0x00000020, 0x00000000, 0x0004002b, 0x00000008,       
    0x00000009, 0x00000003, 0x0004001c, 0x0000000a, 0x00000007, 0x00000009, 0x00040020,       
    0x0000000b, 0x00000006, 0x0000000a, 0x0004003b, 0x0000000b, 0x0000000c, 0x00000006,       
    0x0004002b, 0x00000006, 0x0000000d, 0x00000000, 0x0004002b, 0x00000006, 0x0000000e,       
    0xbf000000, 0x0005002c, 0x00000007, 0x0000000f, 0x0000000d, 0x0000000e, 0x0004002b,       
    0x00000006, 0x00000010, 0x3f000000, 0x0005002c, 0x00000007, 0x00000011, 0x00000010,       
    0x00000010, 0x0005002c, 0x00000007, 0x00000012, 0x0000000e, 0x00000010, 0x0006002c,       
    0x0000000a, 0x00000013, 0x0000000f, 0x00000011, 0x00000012, 0x00040017, 0x00000014,       
    0x00000006, 0x00000003, 0x0004001c, 0x00000015, 0x00000014, 0x00000009, 0x00040020,       
    0x00000016, 0x00000006, 0x00000015, 0x0004003b, 0x00000016, 0x00000017, 0x00000006,       
    0x0004002b, 0x00000006, 0x00000018, 0x3f800000, 0x0006002c, 0x00000014, 0x00000019,       
    0x00000018, 0x0000000d, 0x0000000d, 0x0006002c, 0x00000014, 0x0000001a, 0x0000000d,       
    0x00000018, 0x0000000d, 0x0006002c, 0x00000014, 0x0000001b, 0x0000000d, 0x0000000d,       
    0x00000018, 0x0006002c, 0x00000015, 0x0000001c, 0x00000019, 0x0000001a, 0x0000001b,       
    0x00040017, 0x0000001d, 0x00000006, 0x00000004, 0x0004002b, 0x00000008, 0x0000001e,       
    0x00000001, 0x0004001c, 0x0000001f, 0x00000006, 0x0000001e, 0x0006001e, 0x00000020,       
    0x0000001d, 0x00000006, 0x0000001f, 0x0000001f, 0x00040020, 0x00000021, 0x00000003,       
    0x00000020, 0x0004003b, 0x00000021, 0x00000022, 0x00000003, 0x00040015, 0x00000023,       
    0x00000020, 0x00000001, 0x0004002b, 0x00000023, 0x00000024, 0x00000000, 0x00040020,       
    0x00000025, 0x00000001, 0x00000023, 0x0004003b, 0x00000025, 0x00000026, 0x00000001,       
    0x00040020, 0x00000028, 0x00000006, 0x00000007, 0x00040020, 0x0000002e, 0x00000003,       
    0x0000001d, 0x00040020, 0x00000030, 0x00000003, 0x00000014, 0x0004003b, 0x00000030,       
    0x00000031, 0x00000003, 0x00040020, 0x00000033, 0x00000006, 0x00000014, 0x00050036,       
    0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x0003003e,       
    0x0000000c, 0x00000013, 0x0003003e, 0x00000017, 0x0000001c, 0x0004003d, 0x00000023,       
    0x00000027, 0x00000026, 0x00050041, 0x00000028, 0x00000029, 0x0000000c, 0x00000027,       
    0x0004003d, 0x00000007, 0x0000002a, 0x00000029, 0x00050051, 0x00000006, 0x0000002b,       
    0x0000002a, 0x00000000, 0x00050051, 0x00000006, 0x0000002c, 0x0000002a, 0x00000001,       
    0x00070050, 0x0000001d, 0x0000002d, 0x0000002b, 0x0000002c, 0x0000000d, 0x00000018,       
    0x00050041, 0x0000002e, 0x0000002f, 0x00000022, 0x00000024, 0x0003003e, 0x0000002f,       
    0x0000002d, 0x0004003d, 0x00000023, 0x00000032, 0x00000026, 0x00050041, 0x00000033,       
    0x00000034, 0x00000017, 0x00000032, 0x0004003d, 0x00000014, 0x00000035, 0x00000034,       
    0x0003003e, 0x00000031, 0x00000035, 0x000100fd, 0x00010038,
};

static const uint32_t g_TriangleFragShader[] = 
{
    0x07230203, 0x00010000, 0x000d000a, 0x00000013, 0x00000000, 0x00020011, 0x00000001,       
    0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e,       
    0x00000000, 0x00000001, 0x0007000f, 0x00000004, 0x00000004, 0x6e69616d, 0x00000000,       
    0x00000009, 0x0000000c, 0x00030010, 0x00000004, 0x00000007, 0x00030003, 0x00000002,       
    0x000001c2, 0x000a0004, 0x475f4c47, 0x4c474f4f, 0x70635f45, 0x74735f70, 0x5f656c79,       
    0x656e696c, 0x7269645f, 0x69746365, 0x00006576, 0x00080004, 0x475f4c47, 0x4c474f4f,       
    0x6e695f45, 0x64756c63, 0x69645f65, 0x74636572, 0x00657669, 0x00040005, 0x00000004,       
    0x6e69616d, 0x00000000, 0x00050005, 0x00000009, 0x4374756f, 0x726f6c6f, 0x00000000,       
    0x00050005, 0x0000000c, 0x67617266, 0x6f6c6f43, 0x00000072, 0x00040047, 0x00000009,       
    0x0000001e, 0x00000000, 0x00040047, 0x0000000c, 0x0000001e, 0x00000000, 0x00020013,       
    0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006, 0x00000020,       
    0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00040020, 0x00000008, 0x00000003,       
    0x00000007, 0x0004003b, 0x00000008, 0x00000009, 0x00000003, 0x00040017, 0x0000000a,       
    0x00000006, 0x00000003, 0x00040020, 0x0000000b, 0x00000001, 0x0000000a, 0x0004003b,       
    0x0000000b, 0x0000000c, 0x00000001, 0x0004002b, 0x00000006, 0x0000000e, 0x3f800000,       
    0x00050036, 0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005,       
    0x0004003d, 0x0000000a, 0x0000000d, 0x0000000c, 0x00050051, 0x00000006, 0x0000000f,       
    0x0000000d, 0x00000000, 0x00050051, 0x00000006, 0x00000010, 0x0000000d, 0x00000001,       
    0x00050051, 0x00000006, 0x00000011, 0x0000000d, 0x00000002, 0x00070050, 0x00000007,       
    0x00000012, 0x0000000f, 0x00000010, 0x00000011, 0x0000000e, 0x0003003e, 0x00000009,       
    0x00000012, 0x000100fd, 0x00010038,        
};

static VkPipeline g_TrianglePipeline = VK_NULL_HANDLE;
static VkPipelineLayout g_TrianglePipelineLayout = VK_NULL_HANDLE;
static VkRenderPass g_TriangleRenderPass = VK_NULL_HANDLE;

static VkImage g_ViewportImage = VK_NULL_HANDLE;
static VkDeviceMemory g_ViewportMemory = VK_NULL_HANDLE;
static VkImageView g_ViewportView = VK_NULL_HANDLE;
static VkFramebuffer g_ViewportFramebuffer = VK_NULL_HANDLE;
static VkDescriptorSet g_ViewportDS = VK_NULL_HANDLE;
static VkSampler g_ViewportSampler = VK_NULL_HANDLE;
static uint32_t g_ViewportWidth = 0;
static uint32_t g_ViewportHeight = 0;

static void InitTriangleResources(VkDevice device)
{
    // Create RenderPass
    VkAttachmentDescription attachment = {};
    attachment.format = VK_FORMAT_R8G8B8A8_UNORM;
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkAttachmentReference color_attachment = {};
    color_attachment.attachment = 0;
    color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 1;
    info.pAttachments = &attachment;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 1;
    info.pDependencies = &dependency;
    check_vk_result(vkCreateRenderPass(device, &info, nullptr, &g_TriangleRenderPass));

    // Create Pipeline Layout
    VkPipelineLayoutCreateInfo layout_info = {};
    layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    check_vk_result(vkCreatePipelineLayout(device, &layout_info, nullptr, &g_TrianglePipelineLayout));

    // Create Shader Modules
    VkShaderModule vert_module, frag_module;
    VkShaderModuleCreateInfo vert_info = {};
    vert_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vert_info.codeSize = sizeof(g_TriangleVertShader);
    vert_info.pCode = g_TriangleVertShader;
    check_vk_result(vkCreateShaderModule(device, &vert_info, nullptr, &vert_module));

    VkShaderModuleCreateInfo frag_info = {};
    frag_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    frag_info.codeSize = sizeof(g_TriangleFragShader);
    frag_info.pCode = g_TriangleFragShader;
    check_vk_result(vkCreateShaderModule(device, &frag_info, nullptr, &frag_module));

    VkPipelineShaderStageCreateInfo shaderStages[] = {
        { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_VERTEX_BIT, vert_module, "main", nullptr },
        { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_FRAGMENT_BIT, frag_module, "main", nullptr }
    };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = g_TrianglePipelineLayout;
    pipelineInfo.renderPass = g_TriangleRenderPass;
    pipelineInfo.subpass = 0;

    check_vk_result(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &g_TrianglePipeline));

    vkDestroyShaderModule(device, vert_module, nullptr);
    vkDestroyShaderModule(device, frag_module, nullptr);
}

static void ResizeViewport(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0) return;
    if (g_ViewportWidth == width && g_ViewportHeight == height) return;

    check_vk_result(vkDeviceWaitIdle(device));

    if (g_ViewportFramebuffer) vkDestroyFramebuffer(device, g_ViewportFramebuffer, nullptr);
    if (g_ViewportView) vkDestroyImageView(device, g_ViewportView, nullptr);
    if (g_ViewportImage) vkDestroyImage(device, g_ViewportImage, nullptr);
    if (g_ViewportMemory) vkFreeMemory(device, g_ViewportMemory, nullptr);

    g_ViewportWidth = width;
    g_ViewportHeight = height;

    // Create Image
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    check_vk_result(vkCreateImage(device, &imageInfo, nullptr, &g_ViewportImage));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, g_ViewportImage, &memRequirements);
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
    uint32_t memTypeIndex = -1;
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((memRequirements.memoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
            memTypeIndex = i;
            break;
        }
    }
    allocInfo.memoryTypeIndex = memTypeIndex;
    check_vk_result(vkAllocateMemory(device, &allocInfo, nullptr, &g_ViewportMemory));
    check_vk_result(vkBindImageMemory(device, g_ViewportImage, g_ViewportMemory, 0));

    // Create ImageView
    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = g_ViewportImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    check_vk_result(vkCreateImageView(device, &viewInfo, nullptr, &g_ViewportView));

    // Create Framebuffer
    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = g_TriangleRenderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &g_ViewportView;
    framebufferInfo.width = width;
    framebufferInfo.height = height;
    framebufferInfo.layers = 1;
    check_vk_result(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &g_ViewportFramebuffer));

    // Create Sampler (once)
    if (g_ViewportSampler == VK_NULL_HANDLE) {
        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        check_vk_result(vkCreateSampler(device, &samplerInfo, nullptr, &g_ViewportSampler));
    }

    // Register to ImGui
    if (g_ViewportDS) ImGui_ImplVulkan_RemoveTexture(g_ViewportDS);
    g_ViewportDS = ImGui_ImplVulkan_AddTexture(g_ViewportSampler, g_ViewportView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    
    // Update RenderSystem
    auto rs = std::static_pointer_cast<Destiny::RenderSystem>(Destiny::Engine::GetInstance()->getGraphicsSystem());
    rs->setViewportTextureID(g_ViewportDS);
}

static void RenderTriangle(VkCommandBuffer cmd)
{
    if (g_ViewportFramebuffer == VK_NULL_HANDLE) return;

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = g_TriangleRenderPass;
    renderPassInfo.framebuffer = g_ViewportFramebuffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = {g_ViewportWidth, g_ViewportHeight};

    VkClearValue clearColor = { { {0.0f, 0.0f, 0.0f, 1.0f} } };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, g_TrianglePipeline);

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)g_ViewportWidth;
    viewport.height = (float)g_ViewportHeight;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmd, 0, 1, &viewport);

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = {g_ViewportWidth, g_ViewportHeight};
    vkCmdSetScissor(cmd, 0, 1, &scissor);

    vkCmdDraw(cmd, 3, 1, 0, 0);

    vkCmdEndRenderPass(cmd);
}

#include "Engine/Serializer.h"
#include "Engine/UnSerializer.h"
#include "Engine/FileSystem.h"
#include "Engine/Blob.h"
#include "Physics/PhysicsSystem.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
	{
		return true;
	}
		
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		return 0;
		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
	{
		// Save the new client area dimensions.
		Destiny::Engine::GetInstance()->getGraphicsSystem()->onResize_(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		return 0;
	case WM_EXITSIZEMOVE:
		return 0;
		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		return 0;



	//Mouse
	Destiny::Mouse mouse;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	{
		mouse.code = (Destiny::MouseCode)wParam;
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);
		if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN)
		{
			Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::MousePressed, &mouse);
		}
		if (msg == WM_LBUTTONUP || msg == WM_RBUTTONUP)
		{
			Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::MouseReleased, &mouse);
		}
		if (msg == WM_MOUSEMOVE)
		{
			Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::MouseMoved, &mouse);
		}
		return 0;
	}

	//Keyboard
	Destiny::KeyCode keyCode;
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		keyCode = (Destiny::KeyCode)wParam;
		if (msg == WM_KEYDOWN)
		{
			if (!ImGui::IsAnyItemActive() && !ImGui::IsAnyItemFocused())
			{
				Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::KeyPressed, &keyCode);
			}	
		}
		if (msg == WM_KEYUP)
		{
			if (!ImGui::IsAnyItemActive() && !ImGui::IsAnyItemFocused())
			{
				Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::KeyReleased, &keyCode);
			}
		}
		return 0;
	}

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Application::Application() : 
	m_hwnd(nullptr),
	m_hInstance(nullptr),
	m_viewPortPanel(std::make_shared<ViewPortPanel>()),
	m_scenePanel(std::make_shared<ScenePanel>()),
	m_propertyPanel(std::make_shared<PropertyPanel>()),
	m_assetPanel(std::make_shared<AssetPanel>()),
	m_debugDeferredRenderPanel(std::make_shared<DebugDeferredRenderPanel>()),
	m_debugShadowMapPanel(std::make_shared<DebugShadowMapPanel>()),
	m_graphicsStatPanel(std::make_shared<GraphicsStatPanel>()),
	m_aiPanel(std::make_shared<AiPanel>())
{
	AllocConsole();
	MetaConnect(m_scenePanel, ScenePanel::ChoosedNode, m_propertyPanel, &PropertyPanel::onChoosedNode);
	MetaConnect(m_scenePanel, ScenePanel::ChoosedNode, m_viewPortPanel, &ViewPortPanel::onChoosedNode);
	MetaConnect(m_viewPortPanel, ViewPortPanel::ChoosedNode, m_propertyPanel, &PropertyPanel::onChoosedNode);
}

void Application::initialize(HINSTANCE hInstance)
{
	m_hInstance = hInstance;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Destiny";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return ;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	const unsigned int width = 2000;
	const unsigned int height = 1000;

	RECT R = { 0, 0, width, height };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

	m_hwnd = CreateWindow(L"Destiny", L"Destiny", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
	if (!m_hwnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return ;
	}

	Destiny::Engine::GetInstance()->initialize({ (long long)m_hwnd, 16 });

	//logo
	std::string scenePath = "builtin://texture/favicon.ico";
	auto blobLoader = Destiny::Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(scenePath.c_str());
	scenePath = blobLoader->normalizedPath(blobLoader->createBlobHolder(scenePath));
	
	HICON hIcon = (HICON)LoadImage(NULL, Destiny::Utility::MultiByte2WideChar(scenePath).c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	if (hIcon != NULL)
	{
		SetClassLongPtr(m_hwnd, GCLP_HICON, (LONG_PTR)hIcon);
		SetClassLongPtr(m_hwnd, GCLP_HICONSM, (LONG_PTR)hIcon);
	}

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	initImGui();
}

void Application::run()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//engine render
			Destiny::Engine::GetInstance()->update();

/*
            // Render Triangle Test
            {
                auto graphicsSystem = Destiny::Engine::GetInstance()->getGraphicsSystem();
                auto device = std::static_pointer_cast<Destiny::VulkanDevice>(graphicsSystem->getDevice());
                
                ResizeViewport(device->getVkDevice(), device->getPhysicalDevice(), m_viewPortPanel->getWidth(), m_viewPortPanel->getHeight());
                
                VkCommandPoolCreateInfo poolInfo = {};
                poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
                poolInfo.queueFamilyIndex = device->getGraphicsQueueFamilyIndex();
                poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
                VkCommandPool transientPool;
                check_vk_result(vkCreateCommandPool(device->getVkDevice(), &poolInfo, nullptr, &transientPool));
                
                VkCommandBufferAllocateInfo allocInfo = {};
                allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                allocInfo.commandPool = transientPool;
                allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                allocInfo.commandBufferCount = 1;
                VkCommandBuffer cmd;
                check_vk_result(vkAllocateCommandBuffers(device->getVkDevice(), &allocInfo, &cmd));
                
                VkCommandBufferBeginInfo beginInfo = {};
                beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
                check_vk_result(vkBeginCommandBuffer(cmd, &beginInfo));
                
                RenderTriangle(cmd);
                
                check_vk_result(vkEndCommandBuffer(cmd));
                
                VkSubmitInfo submitInfo = {};
                submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                submitInfo.commandBufferCount = 1;
                submitInfo.pCommandBuffers = &cmd;
                
                check_vk_result(vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE));
                check_vk_result(vkDeviceWaitIdle(device->getVkDevice()));
                
                vkDestroyCommandPool(device->getVkDevice(), transientPool, nullptr);
            }
*/

			//imgui render
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();	

			drawDock();
			
			//static bool show = true;
			//ImGui::ShowDemoWindow(&show);
			//ImGui::ShowMetricsWindow();

			ImGui::Render();
            
            ImDrawData* draw_data = ImGui::GetDrawData();
            auto render_callback = [draw_data](void* cmdBuffer) {
                ImGui_ImplVulkan_RenderDrawData(draw_data, (VkCommandBuffer)cmdBuffer);
            };

			Destiny::Engine::GetInstance()->getGraphicsSystem()->endEvent();

			//present
			Destiny::Engine::GetInstance()->getGraphicsSystem()->present(render_callback);
		}
	}
}

void Application::uninitialize()
{
    auto graphicsSystem = Destiny::Engine::GetInstance()->getGraphicsSystem();
    auto device = std::static_pointer_cast<Destiny::VulkanDevice>(graphicsSystem->getDevice());
    
    check_vk_result(vkDeviceWaitIdle(device->getVkDevice()));
    
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
    
    vkDestroyDescriptorPool(device->getVkDevice(), g_DescriptorPool, nullptr);



	Destiny::Engine::GetInstance()->uninitialize();

	DestroyWindow(m_hwnd);
	UnregisterClassW(L"Destiny", m_hInstance);
}

void Application::initImGui()
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	
	auto blobLoader = Destiny::Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader("builtin://");
	if (blobLoader)
	{
		auto blobHolder1 = blobLoader->createBlobHolder("builtin://fonts/opensans/OpenSans-Bold.ttf");
		auto blobHolder2 = blobLoader->createBlobHolder("builtin://fonts/opensans/OpenSans-Regular.ttf");
		
		if (blobHolder1)
		{
			io.Fonts->AddFontFromFileTTF(blobLoader->normalizedPath(blobHolder1).c_str(), 20);
		}

		if (blobHolder2)
		{
			io.FontDefault = io.Fonts->AddFontFromFileTTF(blobLoader->normalizedPath(blobHolder2).c_str(), 20);
		}
	}

	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	ImGui_ImplWin32_Init(m_hwnd);
	auto graphicsSystem = Destiny::Engine::GetInstance()->getGraphicsSystem();
	
    // Vulkan Init
    auto device = std::static_pointer_cast<Destiny::VulkanDevice>(graphicsSystem->getDevice());
    auto swapChain = std::static_pointer_cast<Destiny::VulkanSwapChain>(graphicsSystem->getSwapChain());

    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        check_vk_result(vkCreateDescriptorPool(device->getVkDevice(), &pool_info, nullptr, &g_DescriptorPool));
    }

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = device->getInstance();
    init_info.PhysicalDevice = device->getPhysicalDevice();
    init_info.Device = device->getVkDevice();
    init_info.QueueFamily = device->getGraphicsQueueFamilyIndex();
    init_info.Queue = device->getGraphicsQueue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = g_DescriptorPool;
    init_info.MinImageCount = swapChain->getImageCount();
    init_info.ImageCount = swapChain->getImageCount();
    init_info.Allocator = nullptr;
    init_info.CheckVkResultFn = check_vk_result;
    
    // Create/Get a compatible RenderPass for ImGui
    Destiny::RenderPassKey key;
    key.colorFormats.push_back(swapChain->getImageFormat());
    key.colorLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD; // ImGui draws on top
    key.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    key.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Keep in attachment optimal for barrier later
    
    init_info.RenderPass = device->getRenderPassCache()->getRenderPass(key);
    init_info.Subpass = 0;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    ImGui_ImplVulkan_Init(&init_info);

    // Initialize Triangle Resources
    // InitTriangleResources(device->getVkDevice());

    // Upload Fonts
    {
        // Use the default command pool and queue from init_info to upload fonts
        if (!ImGui_ImplVulkan_CreateFontsTexture())
        {
            fprintf(stderr, "Failed to create fonts texture\n");
            abort();
        }
        
        // Wait for the upload to finish before starting the loop
        check_vk_result(vkDeviceWaitIdle(device->getVkDevice()));
    }
}

void Application::drawDock()
{
	// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
// In this specific demo, we are not using DockSpaceOverViewport() because:
// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
// - we allow the host window to have padding (when opt_padding == true)
// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
// TL;DR; this demo is more complicated than what you would normally use.
// If we removed all the options we are showcasing, this demo would become:
//     void ShowExampleAppDockSpace()
//     {
//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
//     }

	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	static bool p_open = true;
	ImGui::Begin("DockSpace", &p_open, window_flags);
	if (!opt_padding)
	{
		ImGui::PopStyleVar();
	}
		

	if (opt_fullscreen)
	{
		ImGui::PopStyleVar(2);
	}
		

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	//menu bar
	if (ImGui::BeginMenuBar())
	{
		fileMenu();
		viewMenu();
		toolMenu();
		settingMenu();
		simulationMenu();

		ImGui::EndMenuBar();
	}

	m_viewPortPanel->update();
	m_scenePanel->update();
	m_propertyPanel->update();
	m_assetPanel->update();
	m_debugDeferredRenderPanel->update();
	m_debugShadowMapPanel->update();
	m_graphicsStatPanel->update();
	//m_aiPanel->update();

	ImGui::End();
}

void Application::newScene()
{
#if 0
	auto scene = std::make_shared<Destiny::VisualScene>("New Scene");
	Destiny::Engine::GetInstance()->getSceneManager()->setCurrentScene(scene);
	m_propertyPanel->onChoosedNode(nullptr);
#endif
}

void Application::saveScene()
{
#if 0
	auto scene = Destiny::Engine::GetInstance()->getSceneManager()->getCurrentScene();
    // ...
#endif
}

void Application::openScene()
{
#if 0
    // ...
#endif
}

void Application::openSceneFromServer()
{
#if 0
    // ...
#endif
}

void Application::clearScene()
{
#if 0
	Destiny::Engine::GetInstance()->getSceneManager()->setCurrentScene(nullptr);
	m_propertyPanel->onChoosedNode(nullptr);
#endif
}

void Application::fileMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Open", "CTRL+O"))
		{
			openScene();
		}
		if (ImGui::MenuItem("OpenFromServer", "CTRL+M"))
		{
			openSceneFromServer();
		}
		if (ImGui::MenuItem("New", "CTRL+N"))
		{
			newScene();
		}
		if (ImGui::MenuItem("Save", "CTRL+S"))
		{
			saveScene();
		}
		if (ImGui::MenuItem("Clear", "CTRL+C"))
		{
			clearScene();
		}
		ImGui::EndMenu();
	}

	if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_N)) ||
		(ImGui::IsKeyDown(ImGuiKey_RightCtrl) && ImGui::IsKeyDown(ImGuiKey_N))
		)
	{
		if (!ImGui::IsAnyItemActive() && !ImGui::IsAnyItemFocused())
		{
			newScene();
		}
	}
	if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_S)) ||
		(ImGui::IsKeyDown(ImGuiKey_RightCtrl) && ImGui::IsKeyDown(ImGuiKey_S))
		)
	{
		if (!ImGui::IsAnyItemActive() && !ImGui::IsAnyItemFocused())
		{
			saveScene();

		}
	}
	if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_O)) ||
		(ImGui::IsKeyDown(ImGuiKey_RightCtrl) && ImGui::IsKeyDown(ImGuiKey_O))
		)
	{
		if (!ImGui::IsAnyItemActive() && !ImGui::IsAnyItemFocused())
		{
			openScene();
		}
	}
	if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_M)) ||
		(ImGui::IsKeyDown(ImGuiKey_RightCtrl) && ImGui::IsKeyDown(ImGuiKey_M))
		)
	{
		if (!ImGui::IsAnyItemActive() && !ImGui::IsAnyItemFocused())
		{
			openSceneFromServer();
		}
	}
	if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_C)) ||
		(ImGui::IsKeyDown(ImGuiKey_RightCtrl) && ImGui::IsKeyDown(ImGuiKey_C))
		)
	{
		if (!ImGui::IsAnyItemActive() && !ImGui::IsAnyItemFocused())
		{
			clearScene();
		}
	}

}

void Application::viewMenu()
{
	static bool checkResDebugPanel = false;
	static bool checkResStatisticsInfo = false;
	if (ImGui::BeginMenu("View"))
	{

		if (ImGui::Checkbox("Debug Panel", &checkResDebugPanel))
		{
			m_debugDeferredRenderPanel->setOpen(checkResDebugPanel);
			m_debugShadowMapPanel->setOpen(checkResDebugPanel);
		}

		if (ImGui::Checkbox("StatisticsInfo", &checkResStatisticsInfo))
		{
			Destiny::Engine::GetInstance()->getSceneManager()->showStatisticsInfo(checkResStatisticsInfo);
		}
		ImGui::EndMenu();
	}


	if (ImGui::IsKeyReleased(ImGuiKey_F9))
	{
		if (!ImGui::IsAnyItemActive() && !ImGui::IsAnyItemFocused())
		{
			checkResStatisticsInfo = !checkResStatisticsInfo;
			Destiny::Engine::GetInstance()->getSceneManager()->showStatisticsInfo(checkResStatisticsInfo);
		}
	}
}

void Application::toolMenu()
{
	if (ImGui::BeginMenu("Tool"))
	{
#if 0
		ImGui::Text("Gen hdr cache");
		ImGui::SameLine();
		if (ImGui::Button("Gen hdr cache"))
		{
			Destiny::Texture::Hdr_To_Cube_Irradiance_Prefilter_DDS();
		}
#endif
		ImGui::EndMenu();
	}
}

void Application::settingMenu()
{
	if (ImGui::BeginMenu("Setting"))
	{
		ImGui::Text("DragFloatStep");
		ImGui::SameLine();
		float step = m_propertyPanel->getDragFloatStep();
		if (ImGui::InputFloat("Value", &step, 0.0f, 0.0f, "%.6f"))
		{
			m_propertyPanel->setDragFloatStep(step);
		}

		const char* items[] = { "None", "Translate", "Rotate", "Scale"};
		static int current_item = 1;
		ImGui::Text("GzimoType");
		ImGui::SameLine();
		if (ImGui::Combo("GzimoType", &current_item, items, IM_ARRAYSIZE(items)))
		{
			m_viewPortPanel->setGzimoType(current_item - 1);
		}

		static bool bSsr = false;
#if 0
		if (ImGui::Checkbox("SSR", &bSsr))
		{
			std::static_pointer_cast<Destiny::RenderSystem>(Destiny::Engine::GetInstance()->getGraphicsSystem())->setSsr(bSsr);
		}
#endif

		ImGui::EndMenu();
	}
}

void Application::simulationMenu()
{
	static bool bSimulation = false;
	if (ImGui::BeginMenu("Physics"))
	{
		if (ImGui::Checkbox("Simulation", &bSimulation))
		{
			Destiny::Engine::GetInstance()->getPhysicsSystem()->setSimulation(bSimulation);
		}

		ImGui::EndMenu();
	}
}
