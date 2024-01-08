#include "Model3DComponent.h"
#include "Visual3DComponent.h"
#include "Model3D.h"
#include "Node3D.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "Engine/EventSystem.h"

namespace Destiny
{
	Model3DComponent::Model3DComponent()
	{
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::Update, std::bind(&Model3DComponent::update, this, std::placeholders::_1));
	}

	Model3DComponent::~Model3DComponent()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::Update, std::bind(&Model3DComponent::update, this, std::placeholders::_1));
	}

	void Model3DComponent::set_path(std::string path)
	{
		for (auto& node : get_node()->get_childs())
		{
			node->removeFromParent();
			node.reset();
		}

		m_path = path;
		m_model3D.reset();
		m_model3D = std::make_shared<Model3D>();
		auto blob = std::make_shared<Blob>(m_path.size());
		memcpy_s(blob->getData(), blob->getLength(), m_path.c_str(), blob->getLength());
		auto blobHolder = std::make_shared<BlobHolder>();
		blobHolder->loadSucceeded__(blob);
		m_model3D->initialize(nullptr, blobHolder);
		m_model3D->load();
	}

	void Model3DComponent::set_show(bool show)
	{
		if (m_model3D->isLoadingFinished())
		{
			m_show = show;
			for (const auto& v3dComp : m_model3D->m_visual3DComponents)
			{
				v3dComp.second->set_enable(m_show);
			}
		}
	}

	void Model3DComponent::update(void* data)
	{
		if (m_model3D && m_model3D->isLoadingSucceed())
		{
			for (const auto& v3dComp : m_model3D->m_visual3DComponents)
			{
				auto meshNode = std::make_shared<Node3D>();
				meshNode->set_name(v3dComp.first);
				meshNode->addToParent(get_node());
				meshNode->addComponent(v3dComp.second);
				meshNode->set_transform3D(get_node()->get_transform3D());
			}
			m_model3D->loadFinished_();
		}
	}

	void Model3DComponent::onNodeTransformChanged()
	{
		if (!m_node)
		{
			return;
		}
		auto childNodes = m_node->get_childs();
		for (auto& node: childNodes)
		{
			node->set_transform3D(m_node->get_transform3D());
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<Model3DComponent>("Model3DComponent")
			.constructor<>()
			(
				rttr::policy::ctor::as_raw_ptr
			)
			.property("path", &Model3DComponent::get_path, &Model3DComponent::set_path)
			.property("show", &Model3DComponent::get_show, &Model3DComponent::set_show);
	}
}