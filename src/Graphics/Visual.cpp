#include "Visual.h"
#include "Effect.h"
#include "Mesh.h"
#include "RenderParameters.h"
#include "EffectTechnique.h"
#include "EffectPass.h"
#include "Renderer.h"

namespace Destiny
{
	Visual::Visual() :
		m_effect(nullptr),
		m_mesh(nullptr)
	{

	}

	void Visual::setEffect(std::shared_ptr<Effect> effect)
	{
		if (!effect)
		{
			return;
		}

		m_effect.reset();
		m_effect = effect;
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

	const std::unordered_set<std::shared_ptr<RenderParameters>>& Visual::getRenderParameters()
	{
		if (m_renderParameters.empty())
		{
			createRenderParameters();
		}
		return m_renderParameters;
	}

	void Visual::createRenderParameters()
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