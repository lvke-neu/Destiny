//#pragma once
//#include <memory>
//
//struct ID3D11SamplerState;
//namespace Destiny
//{
//	class VertexShader;
//	class PixelShader;
//	class VertexBuffer;
//	class IndexBuffer;
//	class ConstantBuffer;
//	class Texture;
//	class Camera;
//	class ImagePass
//	{
//	public:
//		ImagePass();
//		~ImagePass();
//		void draw();
//	private:
//		std::unique_ptr<VertexShader> m_vertexShader;
//		std::unique_ptr<PixelShader> m_pixelShader;
//		std::unique_ptr<VertexBuffer> m_vertexBuffer;
//		std::unique_ptr<IndexBuffer> m_indexBuffer;
//		std::unique_ptr<ConstantBuffer> m_constantBuffer;
//		std::unique_ptr<Texture> m_texture;
//		std::unique_ptr<Camera> m_camera;
//		ID3D11SamplerState* m_pSamplerState;
//	};
//}