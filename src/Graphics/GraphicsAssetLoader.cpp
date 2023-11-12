#include "GraphicsAssetLoader.h"
#include "VertexBufferLoader.h"

namespace Destiny
{
	GraphicsAssetLoader::GraphicsAssetLoader() : 
		m_vertexBufferLoader(std::make_shared<VertexBufferLoader>())
	{

	}
}