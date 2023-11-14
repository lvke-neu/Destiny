#pragma once
#include "Engine/Asset.h"

namespace Destiny
{
	class VertexBuffer;
	class IndexBuffer;
	class Mesh : public Asset
	{
	public:
		Mesh(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer);
		virtual ~Mesh();
	public:
		virtual void doLoad() override;
	private:
		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	};
}