#include "Visual3D.h"
#include "Effect.h"
#include "Mesh.h"
#include "RenderParameters.h"
#include "EffectTechnique.h"
#include "EffectPass.h"
#include "Renderer.h"

namespace Destiny
{
	Visual3D::Visual3D() :
		m_effect(nullptr),
		m_mesh(nullptr)
	{

	}

	void Visual3D::setEffect(std::shared_ptr<Effect> effect)
	{
		if (!effect)
		{
			return;
		}

		m_effect.reset();
		m_effect = effect;
	}

	void Visual3D::setMesh(std::shared_ptr<Mesh> mesh)
	{
		if (!mesh)
		{
			return;
		}

		m_mesh.reset();
		m_mesh = mesh;
	}

	std::unordered_set<std::shared_ptr<RenderParameters>>  Visual3D::getRenderParameters()
	{
		if (m_renderParameters.empty())
		{
			createRenderParameters();
		}
		return m_renderParameters;
	}

	void Visual3D::createRenderParameters()
	{
		for (const auto& effectTechnique : m_effect->getEffectTechniques())
		{
			for (const auto& effectPass : effectTechnique->getEffectPasses())
			{
				std::shared_ptr<RenderParameters> renderParameters = std::make_shared<RenderParameters>();
				if (effectPass)
				{
					effectPass->fillRenderParameters(renderParameters);
					if (m_mesh && effectPass->getRenderer())
					{
						m_mesh->fillRenderParameters(renderParameters, effectPass->getRenderer()->getInputSignatureBlob());
					}
				}
				m_renderParameters.insert(renderParameters);
			}	
		}
	}
}