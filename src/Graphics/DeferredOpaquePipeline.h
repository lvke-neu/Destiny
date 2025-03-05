#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"
#include "GraphicsDefine.h"
#include <DirectXMath.h>

namespace Destiny 
{
	class BindRenderTargets;
	class ClearRenderTarget;
	class Visual;
	class DeferredOpaquePipeline : public GraphicsPipeline
	{
	public:
		DeferredOpaquePipeline(std::shared_ptr<RenderSystem> renderSystem);
		virtual ~DeferredOpaquePipeline();
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	public:
		void		onCameraViewChanged(const DirectX::XMMATRIX& cameraView, const DirectX::XMFLOAT3& eyePosition);
		void		onCameraProjChanged(const DirectX::XMMATRIX& cameraProj, float viewportWidth, float viewportHeight);
		void		onDirectionLightChanged(const std::vector<DirectionLight>& directionLights);
		void		onPointLightChanged(const std::vector<PointLight>& pointLights);
	public:
		std::shared_ptr<BindRenderTargets>		m_bindRenderTargets;
	private:
		void onResize(void* data);
	private:
		//albedo
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget0;
		//normal
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget1;
		//mra
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget2;
		//positionW
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget3;
		//normalW
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget4;
		//texcoord
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget5;

		std::shared_ptr<Visual>					m_fullScreenTriangle;
	};
}