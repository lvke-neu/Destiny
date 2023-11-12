#pragma once
#include <memory>

namespace Destiny
{
	class VertexBufferLoader;
	class GraphicsAssetLoader
	{
	public:
		GraphicsAssetLoader();
	public:
		std::shared_ptr<VertexBufferLoader> getVertexBufferLoader() const;
	private:
		std::shared_ptr<VertexBufferLoader> m_vertexBufferLoader;
	};

	inline std::shared_ptr<VertexBufferLoader> GraphicsAssetLoader::getVertexBufferLoader() const
	{
		return m_vertexBufferLoader;
	}
}