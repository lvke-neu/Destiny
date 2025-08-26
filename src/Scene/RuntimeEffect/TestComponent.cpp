#include "TestComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/InputLayout.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshProvider.h"
#include "Graphics/PbrMaterial.h"
#include "Graphics/VertexDefine.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoader.h"

namespace Destiny
{
	std::shared_ptr<Mesh> getMeshSSSS()
	{
		//std::string vbPath = "builtin://mesh/939_vb_24.dat";
		std::string vbPath = "builtin://mesh/1218_72_vb.dat";
		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(vbPath.c_str());
		auto blobHolder = blobLoader->createBlobHolder(vbPath);
		blobHolder->load(0);

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionNormal(), (unsigned int)sizeof(PositionNormal), 0, blobHolder->getBlob());


		//std::string ibPath = "builtin://mesh/939_ib_36.dat";
		std::string ibPath = "builtin://mesh/1218_108_ib.dat";
		auto blobLoader2 = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(ibPath.c_str());
		auto blobHolder2 = blobLoader2->createBlobHolder(ibPath);
		blobHolder2->load(0);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, blobHolder2->getBlob());

		DirectX::BoundingBox aabb;
		DirectX::BoundingBox::CreateFromPoints(aabb, { -FLT_MAX,-FLT_MAX,-FLT_MAX }, { FLT_MAX,FLT_MAX,FLT_MAX });

		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)(blobHolder2->getBlob()->getLength() / sizeof(unsigned short));

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);

		return mesh;
	}

	TestComponent::TestComponent()
	{
		auto renderer = Renderer::Create("builtin://renderer/test.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		auto mesh = getMeshSSSS();
		mesh->load();

		setRenderPass(renderPass);
		setMesh(mesh);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<TestComponent>("TestComponent")
			.constructor<>();
	}
}