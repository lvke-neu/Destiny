#include "MaterialLoader.h"
#include "Material.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"
#include "Texture.h"

namespace Destiny
{
	MaterialLoader::MaterialLoader()
	{

	}

	MaterialLoader::~MaterialLoader()
	{

	}

	void MaterialLoader::loadAsset(std::shared_ptr<Asset> asset)
	{
		m_mtx.lock();
		
		Material* material = dynamic_cast<Material*>(asset.get());
		
		if (!material)
		{
			asset->loadFailed__();
			m_mtx.unlock();
			return;
		}
		material->loadPending_();

		material->m_ambientTexture.reset();
		material->m_diffuseTexture.reset();
		material->m_specularTexture.reset();

		material->m_ambientTexture = Engine::GetInstance()->getGraphicsSystem()->createTexture(material->m_ambientTexturePath.c_str());
		material->m_diffuseTexture = Engine::GetInstance()->getGraphicsSystem()->createTexture(material->m_diffuseTexturePath.c_str());
		material->m_specularTexture = Engine::GetInstance()->getGraphicsSystem()->createTexture(material->m_specularTexturePath.c_str());

		material->m_ambientTexture->load();
		material->m_diffuseTexture->load();
		material->m_specularTexture->load();

		asset->loadSucceeded__();

		m_mtx.unlock();
	}

	std::shared_ptr<Material> MaterialLoader::createAsset()
	{
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->initialize(shared_from_this(), nullptr);
		return material;
	}
}