#include "Model3DComponent.h"
#include "Visual3DComponent.h"
#include "Model3D.h"
#include "Node3D.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobLoader.h"

namespace Destiny
{
	Model3DComponent::Model3DComponent()
	{
		m_model3D = std::make_shared<Model3D>();
		
		const char* path = "assets://Model/2nrtbod1out/2nrtbod1out.obj";
		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobLoader)
		{
			auto blobHolder = blobLoader->createBlobHolder(path);
			m_model3D->initialize(nullptr, blobHolder);
		}
		m_model3D->load();
	}

	void Model3DComponent::onAttachNode()
	{
		for (auto v3dComponent : m_model3D->m_visual3DComponents)
		{
			m_node->addComponent(v3dComponent);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Model3DComponent>("Model3DComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			);
	}
}