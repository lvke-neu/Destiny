#include "Model.h"
#include "ModelLoader.h"
#include "Animation.h"
#include "Engine/BlobHolder.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobLoaderManager.h"
#include "Graphics/VisualComponent.h"
#include "Graphics/SamplerState.h"
#include "Graphics/Texture.h"
#include "Graphics/Mesh.h"

namespace Destiny
{
	std::shared_ptr<ModelLoader> Model::s_modelLoader = std::make_shared<ModelLoader>();
	Model::Model() :
		m_node(nullptr),
		m_mergedAABB{ { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
		m_animator(std::make_shared<Animator>())
	{

	}

	Model::~Model()
	{

	}

	std::shared_ptr<Model> Model::Create(const char* path)
	{
		auto blobLoader = Destiny::Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobLoader)
		{
			auto blobHolder = blobLoader->createBlobHolder(path);

			if (blobHolder)
			{
				std::shared_ptr<Model> model = std::make_shared<Model>();
				model->initialize(s_modelLoader, blobHolder);

				return model;
			}
		}

		return nullptr;
	}

	void Model::onDataLoaded()
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent && visualComponent->getVisual())
			{
				visualComponent->getVisual()->load(0);

				if (visualComponent->getVisual()->getMesh())
				{
					visualComponent->getVisual()->getMesh()->setBoundingBox(m_mergedAABB);
				}
			}	
		}
	}

	void Model::setEnable(bool enable)
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->set_enable(enable);
			}
		}
	}

	void Model::setEnableShadow(bool enableShadow)
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->set_enableShadow(enableShadow);
			}
		}
	}

	void Model::setRenderer(std::string renderer)
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->set_renderer(renderer);
				visualComponent->setConstant("u_hasAnimation", m_animator->m_animations.size() > 0 ? 1.0f : 0.0f);
			}
		}
	}

	void Model::setShadowRenderer(std::string renderer)
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->set_shadowRenderer(renderer);
				visualComponent->setShadowConstant("u_hasAnimation", m_animator->m_animations.size() > 0 ? 1.0f : 0.0f);
			}
		}
	}

	void Model::setRendererCategory(RendererCategory rendererCategory)
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->set_rendererCategory(rendererCategory);
			}
		}
	}

	void Model::setRasterizerDesc(D3D11_RASTERIZER_DESC desc)
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->set_rasterizerDesc(desc);
			}
		}
	}

	void Model::updateAnimation(float deltaTime)
	{
		long long CurrentTimeMillis = GetTickCount64();
		float AnimationTimeSec = ((float)(CurrentTimeMillis - m_animator->startTimeMillis)) / 1000.0f;
		m_animator->calcuFinalTransform(AnimationTimeSec);

		std::vector<DirectX::XMMATRIX> boneTransforms;
		for (const auto& boneInfo : m_animator->m_skeleton->boneInfo)
		{
			boneTransforms.emplace_back(boneInfo.finalTransformation);
		}

		std::shared_ptr<Blob> blob = std::make_shared<Blob>(boneTransforms.size() * sizeof(DirectX::XMMATRIX));
		blob->copyfrom((void*)boneTransforms.data(), blob->getLength());

		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->setConstant("u_boneTransforms", blob);
				visualComponent->getShadowVisual()->setConstant("u_boneTransforms", blob);
			}
		}

	}

	std::string Model::get_animation()
	{
		return m_animator->get_animation();
	}

	void Model::set_animation(std::string animation)
	{
		m_animator->set_animation(animation);
	}

	unsigned int Model::get_animationIndex()
	{
		return m_animator->get_animationIndex();
	}

	void Model::set_animationIndex(unsigned int animationIndex)
	{
		m_animator->set_animationIndex(animationIndex);
	}


	unsigned int Model::get_animationCount()
	{
		return m_animator->get_animationCount();
	}

	void Model::set_animationCount(unsigned int animationCount)
	{
		m_animator->set_animationCount(animationCount);
	}

	float Model::get_animationSpeed()
	{
		return m_animator->get_animationSpeed();
	}

	void Model::set_animationSpeed(float animationSpeed)
	{
		m_animator->set_animationSpeed(animationSpeed);
	}
}