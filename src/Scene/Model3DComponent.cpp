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

	}


	void Model3DComponent::set_path(std::string path)
	{
		m_path = path;
		m_model3D.reset();
		m_model3D = std::make_shared<Model3D>(shared_from_this());
		auto blob = std::make_shared<Blob>(m_path.size());
		memcpy_s(blob->getData(), blob->getLength(), m_path.c_str(), blob->getLength());
		auto blobHolder = std::make_shared<BlobHolder>();
		blobHolder->loadSucceeded__(blob);
		m_model3D->initialize(nullptr, blobHolder);
		m_model3D->load();
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