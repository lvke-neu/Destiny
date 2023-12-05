#include "Visual3DComponent.h"
#include "Engine/Blob.h"
#include "Engine/Engine.h"
#include "Graphics/Visual3D.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/Material.h"
#include "Graphics/SamplerState.h"
#include "Graphics/Texture.h"
#include "Node3D.h"

namespace Destiny
{
	Visual3DComponent::Visual3DComponent() :
		m_visual3D(std::make_shared<Visual3D>()),
		m_rasterizerStateDesc(RasterizerState::Default_Rasterizer_Desc),
		m_depthStencilStateDesc(DepthStencilState::Default_DepthStencil_Desc),
		m_blendStateDesc(BlendState::Default_BlendState_Desc),
		m_color(Color::Green),
		m_worldMatrix(std::make_shared<ConstantBuffer<XMMATRIX>>()),
		m_material(Engine::GetInstance()->getGraphicsSystem()->createMaterial()),
		m_materiaColor(std::make_shared<ConstantBuffer<MateriaColor>>())
	{
		std::shared_ptr<Blob> data = nullptr;

		data.reset(new Blob(sizeof(D3D11_RASTERIZER_DESC)));
		memcpy_s(data->getData(), data->getLength(), &m_rasterizerStateDesc, data->getLength());
		auto rasterizerState = Engine::GetInstance()->getGraphicsSystem()->createRasterizerState(data);
		rasterizerState->load(0);
		m_visual3D->setRasterizerState(rasterizerState);

		
		data.reset(new Blob(sizeof(D3D11_DEPTH_STENCIL_DESC)));
		memcpy_s(data->getData(), data->getLength(), &m_depthStencilStateDesc, data->getLength());
		auto depthStencilState = Engine::GetInstance()->getGraphicsSystem()->createDepthStencilState(data);
		depthStencilState->load(0);
		m_visual3D->setDepthStencilState(depthStencilState);

	
		data.reset(new Blob(sizeof(D3D11_BLEND_DESC)));
		memcpy_s(data->getData(), data->getLength(), &m_blendStateDesc, data->getLength());
		auto blendState = Engine::GetInstance()->getGraphicsSystem()->createBlendState(data);
		blendState->load(0);
		m_visual3D->setBlendState(blendState);

		m_visual3D->registerBeforeDrawCommands(std::bind(&Visual3DComponent::beforeDrawCommand, this));

		data.reset(new Blob(sizeof(D3D11_SAMPLER_DESC)));
		memcpy_s(data->getData(), data->getLength(), &SamplerState::Default_SamplerState_Desc, data->getLength());
		auto samplerState = Engine::GetInstance()->getGraphicsSystem()->createSamplerState(data);
		samplerState->load(0);
		m_material->set_ambientSamplerState(samplerState);
		m_material->set_diffuseSamplerState(samplerState);
		m_material->set_specularSamplerState(samplerState);
	}

	void Visual3DComponent::set_rasterizerStateDesc(D3D11_RASTERIZER_DESC rasterizerStateDesc)
	{
		m_rasterizerStateDesc = rasterizerStateDesc;
		std::shared_ptr<Blob> data = nullptr;
		data.reset(new Blob(sizeof(D3D11_RASTERIZER_DESC)));
		memcpy_s(data->getData(), data->getLength(), &m_rasterizerStateDesc, data->getLength());
		auto rasterizerState = Engine::GetInstance()->getGraphicsSystem()->createRasterizerState(data);
		rasterizerState->load(0);
		m_visual3D->setRasterizerState(rasterizerState);
	}

	void Visual3DComponent::set_depthStencilStateDesc(D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc)
	{
		m_depthStencilStateDesc = depthStencilStateDesc;
		std::shared_ptr<Blob> data = nullptr;
		data.reset(new Blob(sizeof(D3D11_DEPTH_STENCIL_DESC)));
		memcpy_s(data->getData(), data->getLength(), &m_depthStencilStateDesc, data->getLength());
		auto depthStencilState = Engine::GetInstance()->getGraphicsSystem()->createDepthStencilState(data);
		depthStencilState->load(0);
		m_visual3D->setDepthStencilState(depthStencilState);
	}

	void Visual3DComponent::set_blendStateDesc(D3D11_BLEND_DESC blendStateDesc)
	{
		m_blendStateDesc = blendStateDesc;
		std::shared_ptr<Blob> data = nullptr;
		data.reset(new Blob(sizeof(D3D11_BLEND_DESC)));
		memcpy_s(data->getData(), data->getLength(), &m_blendStateDesc, data->getLength());
		auto blendState = Engine::GetInstance()->getGraphicsSystem()->createBlendState(data);
		blendState->load(0);
		m_visual3D->setBlendState(blendState);
	}

	void Visual3DComponent::onAttachNode()
	{
		m_worldMatrix->update(XMMatrixTranspose(m_node->get_transform3D().getWorldMatrix()));
	}

	void Visual3DComponent::onNodeTransformChanged()
	{
		m_worldMatrix->update(XMMatrixTranspose(m_node->get_transform3D().getWorldMatrix()));
	}

	void Visual3DComponent::beforeDrawCommand()
	{
		auto immediateContext = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext();
		immediateContext->VSSetConstantBuffers(2, 1, m_worldMatrix->getConstantBuffer());

		if (!m_material || !m_material->isLoadingSucceed())
		{
			return;
		}
		
		MateriaColor materialColor;
		memcpy_s(&materialColor.ambientColor, sizeof(XMFLOAT4), m_material->get_ambientColor().toFloat(), sizeof(XMFLOAT4));
		memcpy_s(&materialColor.diffuseColor, sizeof(XMFLOAT4), m_material->get_diffuseColor().toFloat(), sizeof(XMFLOAT4));
		memcpy_s(&materialColor.specularColor, sizeof(XMFLOAT4), m_material->get_specularColor().toFloat(), sizeof(XMFLOAT4));

		m_materiaColor->update(materialColor);
		immediateContext->PSSetConstantBuffers(3, 1, m_materiaColor->getConstantBuffer());

		if (m_material->get_ambientTexture() && m_material->get_ambientTexture()->isLoadingSucceed()
			&& m_material->get_ambientSamplerState() && m_material->get_ambientSamplerState()->isLoadingSucceed())
		{
			immediateContext->PSSetSamplers(0, 1, m_material->get_ambientSamplerState()->getSamplerState());
			immediateContext->PSSetShaderResources(0, 1, m_material->get_ambientTexture()->getShaderResourceView());
		}
		if (m_material->get_diffuseTexture() && m_material->get_diffuseTexture()->isLoadingSucceed()
			&& m_material->get_diffuseSamplerState() && m_material->get_diffuseSamplerState()->isLoadingSucceed())
		{
			immediateContext->PSSetSamplers(1, 1, m_material->get_diffuseSamplerState()->getSamplerState());
			immediateContext->PSSetShaderResources(1, 1, m_material->get_diffuseTexture()->getShaderResourceView());
		}
		if (m_material->get_specularTexture() && m_material->get_specularTexture()->isLoadingSucceed()
			&& m_material->get_specularSamplerState() && m_material->get_specularSamplerState()->isLoadingSucceed())
		{
			immediateContext->PSSetSamplers(1, 1, m_material->get_specularSamplerState()->getSamplerState());
			immediateContext->PSSetShaderResources(1, 1, m_material->get_specularTexture()->getShaderResourceView());
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Visual3DComponent>("Visual3DComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			)
			.property("rasterizerStateDesc", &Visual3DComponent::get_rasterizerStateDesc, &Visual3DComponent::set_rasterizerStateDesc)
			.property("depthStencilStateDesc", &Visual3DComponent::get_depthStencilStateDesc, &Visual3DComponent::set_depthStencilStateDesc)
			.property("blendStateDesc", &Visual3DComponent::get_blendStateDesc, &Visual3DComponent::set_blendStateDesc)
			.property("color", &Visual3DComponent::get_color, &Visual3DComponent::set_color);

		rttr::registration::class_<D3D11_RASTERIZER_DESC>("D3D11_RASTERIZER_DESC")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			)
			.property("FillMode", &D3D11_RASTERIZER_DESC::FillMode)
			.property("CullMode", &D3D11_RASTERIZER_DESC::CullMode)
			.property("FrontCounterClockwise", &D3D11_RASTERIZER_DESC::FrontCounterClockwise)
			.property("DepthBias", &D3D11_RASTERIZER_DESC::DepthBias)
			.property("DepthBiasClamp", &D3D11_RASTERIZER_DESC::DepthBiasClamp)
			.property("SlopeScaledDepthBias", &D3D11_RASTERIZER_DESC::SlopeScaledDepthBias)
			.property("DepthClipEnable", &D3D11_RASTERIZER_DESC::DepthClipEnable)
			.property("ScissorEnable", &D3D11_RASTERIZER_DESC::ScissorEnable)
			.property("MultisampleEnable", &D3D11_RASTERIZER_DESC::MultisampleEnable)
			.property("AntialiasedLineEnable", &D3D11_RASTERIZER_DESC::AntialiasedLineEnable);
		rttr::registration::class_<D3D11_DEPTH_STENCIL_DESC>("D3D11_DEPTH_STENCIL_DESC")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
				)
			.property("DepthEnable", &D3D11_DEPTH_STENCIL_DESC::DepthEnable)
			.property("DepthWriteMask", &D3D11_DEPTH_STENCIL_DESC::DepthWriteMask)
			.property("DepthFunc", &D3D11_DEPTH_STENCIL_DESC::DepthFunc)
			.property("StencilEnable", &D3D11_DEPTH_STENCIL_DESC::StencilEnable)
			.property("StencilReadMask", &D3D11_DEPTH_STENCIL_DESC::StencilReadMask)
			.property("StencilWriteMask", &D3D11_DEPTH_STENCIL_DESC::StencilWriteMask)
			.property("FrontFace", &D3D11_DEPTH_STENCIL_DESC::FrontFace)
			.property("BackFace", &D3D11_DEPTH_STENCIL_DESC::BackFace);
		rttr::registration::class_<D3D11_BLEND_DESC>("D3D11_BLEND_DESC")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
				)
			.property("AlphaToCoverageEnable", &D3D11_BLEND_DESC::AlphaToCoverageEnable)
			.property("IndependentBlendEnable", &D3D11_BLEND_DESC::IndependentBlendEnable)
			.property("RenderTarget", &D3D11_BLEND_DESC::RenderTarget);
	}
}