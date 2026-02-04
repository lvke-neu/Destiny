#pragma once

namespace Destiny
{
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;
        virtual void clearRenderTarget(float r, float g, float b, float a) = 0;
        
        // Temporary DX11-like stubs
        virtual long Map(void* resource, unsigned int subresource, unsigned int mapType, unsigned int mapFlags, void* mappedResource) { return 0; }
        virtual void Unmap(void* resource, unsigned int subresource) {}
        
        virtual void VSSetConstantBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* ppConstantBuffers) {}
        virtual void PSSetConstantBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* ppConstantBuffers) {}
        virtual void GSSetConstantBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* ppConstantBuffers) {}
        virtual void HSSetConstantBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* ppConstantBuffers) {}
        virtual void DSSetConstantBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* ppConstantBuffers) {}
        virtual void CSSetConstantBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* ppConstantBuffers) {}

        virtual void VSSetSamplers(unsigned int startSlot, unsigned int numSamplers, void* const* ppSamplers) {}
        virtual void PSSetSamplers(unsigned int startSlot, unsigned int numSamplers, void* const* ppSamplers) {}
        virtual void GSSetSamplers(unsigned int startSlot, unsigned int numSamplers, void* const* ppSamplers) {}
        virtual void HSSetSamplers(unsigned int startSlot, unsigned int numSamplers, void* const* ppSamplers) {}
        virtual void DSSetSamplers(unsigned int startSlot, unsigned int numSamplers, void* const* ppSamplers) {}
        virtual void CSSetSamplers(unsigned int startSlot, unsigned int numSamplers, void* const* ppSamplers) {}

        virtual void VSSetShaderResources(unsigned int startSlot, unsigned int numViews, void* const* ppShaderResourceViews) {}
        virtual void PSSetShaderResources(unsigned int startSlot, unsigned int numViews, void* const* ppShaderResourceViews) {}
        virtual void GSSetShaderResources(unsigned int startSlot, unsigned int numViews, void* const* ppShaderResourceViews) {}
        virtual void HSSetShaderResources(unsigned int startSlot, unsigned int numViews, void* const* ppShaderResourceViews) {}
        virtual void DSSetShaderResources(unsigned int startSlot, unsigned int numViews, void* const* ppShaderResourceViews) {}
        virtual void CSSetShaderResources(unsigned int startSlot, unsigned int numViews, void* const* ppShaderResourceViews) {}

        virtual void DrawIndexed(unsigned int indexCount, unsigned int startIndexLocation, int baseVertexLocation) {}

        virtual void CopyResource(void* pDstResource, void* pSrcResource) {}

        // Input Assembler
        virtual void IASetInputLayout(void* pInputLayout) {}
        virtual void IASetVertexBuffers(unsigned int startSlot, unsigned int numBuffers, void* const* ppVertexBuffers, const unsigned int* pStrides, const unsigned int* pOffsets) {}
        virtual void IASetIndexBuffer(void* pIndexBuffer, int format, unsigned int offset) {}
        virtual void IASetPrimitiveTopology(int topology) {}

        // Shaders
        virtual void VSSetShader(void* pVertexShader, void* const* ppClassInstances, unsigned int numClassInstances) {}
        virtual void PSSetShader(void* pPixelShader, void* const* ppClassInstances, unsigned int numClassInstances) {}
        virtual void GSSetShader(void* pGeometryShader, void* const* ppClassInstances, unsigned int numClassInstances) {}
        virtual void HSSetShader(void* pHullShader, void* const* ppClassInstances, unsigned int numClassInstances) {}
        virtual void DSSetShader(void* pDomainShader, void* const* ppClassInstances, unsigned int numClassInstances) {}
        virtual void CSSetShader(void* pComputeShader, void* const* ppClassInstances, unsigned int numClassInstances) {}

        // Rasterizer
        virtual void RSSetState(void* pRasterizerState) {}
        virtual void RSSetViewports(unsigned int numViewports, const void* pViewports) {}
        virtual void RSSetScissorRects(unsigned int numRects, const void* pRects) {}

        // Output Merger
        virtual void OMSetRenderTargets(unsigned int numViews, void* const* ppRenderTargetViews, void* pDepthStencilView) {}
        virtual void OMSetDepthStencilState(void* pDepthStencilState, unsigned int stencilRef) {}
        virtual void OMSetBlendState(void* pBlendState, const float blendFactor[4], unsigned int sampleMask) {}
    };
}
