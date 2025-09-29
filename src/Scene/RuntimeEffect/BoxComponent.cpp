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
#include "Graphics/Texture.h"

namespace Destiny
{
	BoxComponent::BoxComponent()
	{
		//auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader("http://");
		//auto blobHolder = blobLoader->createBlobHolder("http://www.baidu.com");
		//blobHolder->load(0);

		//auto httpTexture = Texture::Create("http://fastly.picsum.photos/id/10/1920/1080.jpg?hmac=Hs_xUcCc7BNrD6fseq1fdN2AC_uSWaywG7V7uh_6fTY");
		//httpTexture->load();

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

	RTTR_REGISTRATION
	{
		rttr::registration::class_<BoxComponent>("BoxComponent")
			.constructor<>();
	}
}