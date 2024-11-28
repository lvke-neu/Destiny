#include "Visual.h"
#include "RenderPass.h"
#include "Mesh.h"
#include "Renderer.h"
#include "DrawCommand.h"
#include "DrawParameters.h"

namespace Destiny
{
	Visual::Visual() :
		m_renderPass(nullptr),
		m_mesh(nullptr),
		m_drawParameters(std::make_shared<DrawParameters>())
	{

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

	void Visual::upDrawParameters()
	{
		if (m_renderPass)
		{
			m_renderPass->fillDrawParameters(m_drawParameters);
			if (m_mesh && m_renderPass->getRenderer())
			{
				m_mesh->fillDrawParameters(m_drawParameters, m_renderPass->getRenderer()->getInputSignatureBlob());
			}
		}

		addDrawParameter(m_drawParameters);
	}
}