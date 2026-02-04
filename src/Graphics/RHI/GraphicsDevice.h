#pragma once
#include <memory>

namespace Destiny
{
    class GraphicsContext;
    class SwapChain;

    class GraphicsDevice
    {
    public:
        virtual ~GraphicsDevice() = default;

        virtual void initialize(void* windowHandle) = 0;
        virtual std::shared_ptr<GraphicsContext> getImmediateContext() = 0;
        virtual std::shared_ptr<SwapChain> createSwapChain(void* windowHandle, int width, int height) = 0;

        // Temporary DX11-like stubs to satisfy compilation of legacy code
        // In a real RHI, these would return abstract RHI objects, not void* or raw D3D pointers
        virtual long CreateBuffer(const void* desc, const void* data, void** ppBuffer) { return 0; }
        virtual void DestroyBuffer(void* buffer) {}
        virtual long CreateInputLayout(const void* elementDescs, unsigned int numElements, const void* shaderBytecode, size_t bytecodeLength, void** ppInputLayout) { return 0; }
        
        virtual long CreateTexture2D(const void* desc, const void* initialData, void** ppTexture2D) { return 0; }
        virtual void DestroyTexture(void* texture) {}
        virtual long CreateDepthStencilView(void* resource, const void* desc, void** ppDepthStencilView) { return 0; }
        virtual long CreateShaderResourceView(void* resource, const void* desc, void** ppSRView) { return 0; }
        virtual long CreateComputeShader(const void* shaderBytecode, size_t bytecodeLength, void* classLinkage, void** ppComputeShader) { return 0; }
        virtual long CreateRenderTargetView(void* resource, const void* desc, void** ppRTView) { return 0; }
        virtual long CreateSamplerState(const void* samplerDesc, void** ppSamplerState) { return 0; }
        virtual void DestroySamplerState(void* samplerState) {}
        virtual long CreateUnorderedAccessView(void* resource, const void* desc, void** ppUAView) { return 0; }
        virtual long CreateVertexShader(const void* shaderBytecode, size_t bytecodeLength, void* classLinkage, void** ppVertexShader) { return 0; }
        virtual long CreatePixelShader(const void* shaderBytecode, size_t bytecodeLength, void* classLinkage, void** ppPixelShader) { return 0; }
        virtual long CreateGeometryShader(const void* shaderBytecode, size_t bytecodeLength, void* classLinkage, void** ppGeometryShader) { return 0; }
        virtual long CreateHullShader(const void* shaderBytecode, size_t bytecodeLength, void* classLinkage, void** ppHullShader) { return 0; }
        virtual long CreateDomainShader(const void* shaderBytecode, size_t bytecodeLength, void* classLinkage, void** ppDomainShader) { return 0; }
        
        virtual long CreateRasterizerState(const void* rasterizerDesc, void** ppRasterizerState) { return 0; }
        virtual long CreateDepthStencilState(const void* depthStencilDesc, void** ppDepthStencilState) { return 0; }
        virtual long CreateBlendState(const void* blendStateDesc, void** ppBlendState) { return 0; }
        virtual void DestroyRasterizerState(void* state) {}
        virtual void DestroyDepthStencilState(void* state) {}
        virtual void DestroyBlendState(void* state) {}

        virtual void* getNativeDevice() { return nullptr; }
    };
}
