#include "Visual.h"
#include "RenderPass.h"
#include "Mesh.h"
#include "Renderer.h"
#include "DrawCommand.h"
#include "DrawParameters.h"
#include "VisualScene.h"
#include "Engine/Component.h"
#include "BindRenderTargets.h"
#include "GraphicsPipeline/GraphicsCommandList.h"

namespace Destiny
{
	Visual::Visual() :
		m_renderPass(std::make_shared<RenderPass>()),
		m_mesh(nullptr),
		m_drawParameters(std::make_shared<DrawParameters>()),
		m_component(nullptr),
		m_constantsChanged(false),
		m_texturesChanged(false),
		m_samplerStatesrChanged(false),
		m_renderPassChanged(false),
		m_meshChanged(false),
		m_rendererConstantsChanged(false),
		m_visualCategory(VisualCategory::RenderToScene)
	{
		addDrawParameter(m_drawParameters);
	}

	std::string Visual::get_renderer()
	{
		return m_renderPass ? m_renderPass->get_renderer() : "";
	}

	void Visual::set_renderer(std::string renderer)
	{
		if (m_renderPass)
		{
			m_renderPass->set_renderer(renderer);
		}
	}

	RendererCategory Visual::get_rendererCategory()
	{
		return m_renderPass ? m_renderPass->getRendererCategory() : RendererCategory::None;
	}

	void Visual::set_rendererCategory(RendererCategory rendererCategory)
	{
		if (m_renderPass)
		{
			m_renderPass->setRendererCategory(rendererCategory);
		}
	}

	D3D11_RASTERIZER_DESC Visual::get_rasterizerDesc()
	{
		if (m_renderPass)
		{
			return m_renderPass->get_rasterizerDesc();
		}
		return D3D11_RASTERIZER_DESC();
	}

	void Visual::set_rasterizerDesc(D3D11_RASTERIZER_DESC desc)
	{
		if (m_renderPass)
		{
			m_renderPass->set_rasterizerDesc(desc);
		}
	}

	D3D11_DEPTH_STENCIL_DESC Visual::get_depthStencilDesc()
	{
		if (m_renderPass)
		{
			return m_renderPass->get_depthStencilDesc();
		}
		return D3D11_DEPTH_STENCIL_DESC();
	}

	void Visual::set_depthStencilDesc(D3D11_DEPTH_STENCIL_DESC desc)
	{
		if (m_renderPass)
		{
			m_renderPass->set_depthStencilDesc(desc);
		}
	}

	D3D11_BLEND_DESC Visual::get_blendDesc()
	{
		if (m_renderPass)
		{
			return m_renderPass->get_blendDesc();
		}
		return D3D11_BLEND_DESC();
	}

	void Visual::set_blendDesc(D3D11_BLEND_DESC desc)
	{
		if (m_renderPass)
		{
			return m_renderPass->set_blendDesc(desc);
		}
	}

	std::shared_ptr<Material> Visual::get_material()
	{
		return m_material;
	}

	void Visual::set_material(std::shared_ptr<Material> material)
	{
		m_material = material;
		if (m_material)
		{
			m_material->bind(shared_from_this());
		}
	}

	void Visual::setRenderPass(std::shared_ptr<RenderPass> renderPass)
	{
		if (!renderPass)
		{
			return;
		}

		m_renderPass.reset();
		m_renderPass = renderPass;
		m_renderPassChanged = true;
	}

	void Visual::setMesh(std::shared_ptr<Mesh> mesh)
	{
		if (!mesh)
		{
			return;
		}

		m_mesh.reset();
		m_mesh = mesh;
		m_meshChanged = true;
	}

	void Visual::updateDrawParameters()
	{
		if (m_renderPass)
		{
			if (m_constantBuffers.empty())
			{
				m_renderPass->fillConstantBuffers(m_variableLinkConstant, m_constantBuffers);
			}

			if (m_textures.empty())
			{
				m_renderPass->fillTextures(m_textures);
			}

			if (m_samplerStates.empty())
			{
				m_renderPass->fillSamplerStates(m_samplerStates);
			}

			if (m_rendererConstantsChanged)
			{
				m_renderPass->modifyConstantBuffersByDifference(m_constantBuffers);
				m_drawParameters->constantBuffers = m_constantBuffers;
				m_rendererConstantsChanged = false;
			}

			if (m_constantsChanged)
			{
				for (const auto& constant : m_constants)
				{
					auto iter1 = m_variableLinkConstant.find(constant.first);
					if (iter1 == m_variableLinkConstant.end())
					{
						continue;
					}

					auto iter2 = m_constantBuffers.find(iter1->second);
					if (iter2 == m_constantBuffers.end() || iter2->second == nullptr)
					{
						continue;
					}

					iter2->second->setVariable(constant.first, constant.second);
				}
				m_drawParameters->constantBuffers = m_constantBuffers;
				m_constantsChanged = false;
			}

			if (m_texturesChanged)
			{
				for (const auto& texture : m_visualTextures)
				{
					auto iter = m_textures.find(texture.first);
					if (iter == m_textures.end())
					{
						continue;
					}

					iter->second.second = texture.second;
				}
				m_drawParameters->textures = m_textures;
				m_texturesChanged = false;
			}

			if (m_samplerStatesrChanged)
			{
				for (const auto& samplerState : m_visualSamplerStates)
				{
					auto iter = m_samplerStates.find(samplerState.first);
					if (iter == m_samplerStates.end())
					{
						continue;
					}

					iter->second.second = samplerState.second;
				}
				m_drawParameters->samplerStates = m_samplerStates;
				m_samplerStatesrChanged = false;
			}

			//if (m_renderPassChanged || m_meshChanged)
			//{
			//	m_renderPass->fillDrawParameters(m_drawParameters);
			//	m_renderPassChanged = false;
			//	if (m_mesh && m_renderPass->getRenderer())
			//	{
			//		m_mesh->fillDrawParameters(m_drawParameters, m_renderPass->getRenderer()->getInputSignatureBlob());
			//		m_meshChanged = false;
			//	}
			//}

			m_renderPass->fillDrawParameters(m_drawParameters);
			if (m_mesh && m_renderPass->getRenderer())
			{
				m_mesh->fillDrawParameters(m_drawParameters, m_renderPass->getRenderer()->getInputSignatureBlob());
			}
		}

		//clearDrawParameter();
	}

	void Visual::load(int priority)
	{
		if (m_renderPass)
		{
			m_renderPass->load(priority);
		}
		if (m_mesh)
		{
			m_mesh->load(priority);
		}
	}

	void Visual::setShaderResource(const char* name, std::shared_ptr<Texture> texture)
	{
		m_visualTextures[name] = texture;
		m_texturesChanged = true;
	}

	void Visual::setSamplerSate(const char* name, std::shared_ptr<SamplerState> samplerState)
	{
		m_visualSamplerStates[name] = samplerState;
		m_samplerStatesrChanged = true;
	}
}