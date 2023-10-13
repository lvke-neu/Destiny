#pragma once
#include <memory>

namespace Destiny
{
	class VertexShader;
	class PixelShader;
	class VertexBuffer;
	class IndexBuffer;
	class ConstantBuffer;
	class ImagePass
	{
	public:
		ImagePass();
		~ImagePass();
		void draw();
		void onResize(void* data);
	private:
		std::unique_ptr<VertexShader> m_vertexShader;
		std::unique_ptr<PixelShader> m_pixelShader;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
		std::unique_ptr<ConstantBuffer> m_constantBuffer;
	};
}