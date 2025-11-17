#include "BoxComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"

#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/FileSystem.h"
#include "Graphics/Texture.h"


namespace Destiny
{
	static std::shared_ptr<BlobHolder> blobHolder;
	BoxComponent::BoxComponent()
	{
		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader("http://");
		blobHolder = blobLoader->createBlobHolder("http://localhost:8000/Bee.glb");
		blobHolder->load();


		auto renderer = Renderer::Create("builtin://renderer/forward_pbr.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = MeshProvider::Create_Box_PositionNormalTexcoord();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
		set_material(PbrMaterial::Create_Wall());


		auto shadowRenderer = Renderer::Create("builtin://renderer/forward_pbr_shadow.hlsl");
		shadowRenderer->load(0);
		auto shadowRenderStates = std::make_shared<RenderStates>();
		shadowRenderStates->load();
		auto shadowRenderPass = std::make_shared<RenderPass>();
		shadowRenderPass->setRenderer(shadowRenderer);
		shadowRenderPass->setRenderStates(shadowRenderStates);

		setShadowRenderPass(shadowRenderPass);
		setShadowMesh(mesh);
	}

	void BoxComponent::onUpdate(float deltaTime)
	{
		static bool flag = true;
		if(flag && blobHolder->isLoadingSucceed())
		{
			flag = false;
			FileSystem::WriteBlob("D:\\C++Project\\Destiny\\build\\Debug\\builtin\\Bee.glb", blobHolder->getBlob());
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<BoxComponent>("BoxComponent")
			.constructor<>();
	}
}