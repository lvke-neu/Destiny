#pragma once
#include <memory>

struct ID3D11DeviceContext;
namespace Destiny
{
	class VertexBuffer;
	class IndexBuffer;
	class InputLayout;
	class VertexShader;
	class PixelShader;
	class RasterizerState;
	class DepthStencilState;
	class BlendState;
	class Visual3D
	{
	public:
		Visual3D();
		~Visual3D();
	public:
		void draw();
	private:
		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		std::shared_ptr<InputLayout> m_inputLayout;
		std::shared_ptr<VertexShader> m_vertexShader;
		std::shared_ptr<PixelShader> m_pixelShader;
		std::shared_ptr<RasterizerState> m_rasterizerState;
		std::shared_ptr<DepthStencilState> m_depthStencilState;
		std::shared_ptr<BlendState> m_blendState;

		ID3D11DeviceContext* m_immediateContext;
	};
}