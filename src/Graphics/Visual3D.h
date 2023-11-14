#pragma once
#include <memory>

struct ID3D11CommandList;
namespace Destiny
{
	class VertexBuffer;
	class IndexBuffer;
	class InputLayout;
	class VertexShader;
	class PixelShader;
	class Visual3D
	{
	public:
		Visual3D();
		~Visual3D();
	public:
		void draw();
	public:
		ID3D11CommandList* getCommmandList();
	private:
		ID3D11CommandList* m_commandList;

		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		std::shared_ptr<InputLayout> m_inputLayout;
		std::shared_ptr<VertexShader> m_vertexShader;
		std::shared_ptr<PixelShader> m_pixelShader;
	};
}