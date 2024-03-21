#pragma once
#include "Engine/Asset.h"
#include <string>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
namespace Destiny
{
	class BlobHolder;
	class Renderer : public Asset
	{
	public:
		Renderer(const char* path);
		virtual ~Renderer();
	public:
		virtual void doLoad() override;
	private:
		bool createVertexShader();
		bool createPixelShader();
	private:
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		std::shared_ptr<BlobHolder> m_blobHolder;
	};
}