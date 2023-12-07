#include "Model3DComponent.h"
#include "Visual3DComponent.h"
#include "Model3D.h"
#include "Node3D.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"

namespace Destiny
{
	Model3DComponent::Model3DComponent()
	{
		m_model3D = std::make_shared<Model3D>();
		
		std::string path ="assets://Model/Madara_Uchiha/Madara_Uchiha.obj";
		auto blob = std::make_shared<Blob>(path.size());
		memcpy_s(blob->getData(), blob->getLength(), path.c_str(), blob->getLength());
		auto blobHolder = std::make_shared<BlobHolder>();
		blobHolder->loadSucceeded__(blob);
		m_model3D->initialize(nullptr, blobHolder);
		m_model3D->load(0);
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