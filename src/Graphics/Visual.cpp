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
		m_component(nullptr)
	{

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
		if (m_renderPass)
		{
			return m_renderPass->getMaterial();
		}
		return nullptr;
	}

	void Visual::set_material(std::shared_ptr<Material> material)
	{
		if (m_renderPass)
		{
			m_renderPass->setMaterial(material);
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
	}

	void Visual::setMesh(std::shared_ptr<Mesh> mesh)
	{
		if (!mesh)
		{
			return;
		}

		m_mesh.reset();
		m_mesh = mesh;
	}

	void Visual::updateDrawParameters()
	{
		if (m_renderPass)
		{
			m_renderPass->fillDrawParameters(m_drawParameters);
			if (m_mesh && m_renderPass->getRenderer())
			{
				m_mesh->fillDrawParameters(m_drawParameters, m_renderPass->getRenderer()->getInputSignatureBlob());
			}
		}

		//if (!m_drawParameters->beforeDrawCommandList)
		//{
		//	m_drawParameters->beforeDrawCommandList = std::make_shared<GraphicsCommandList>();
		//	auto scene = std::dynamic_pointer_cast<VisualScene>(m_component->get_scene());
		//	if (scene)
		//	{
		//		m_drawParameters->beforeDrawCommandList->addGraphicsCommand(scene->m_bindRenderTargets);
		//	}
		//}

		addDrawParameter(m_drawParameters);
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
}