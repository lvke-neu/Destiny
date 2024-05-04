#pragma once
#include "Engine/Asset.h"
#include <string>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
namespace Destiny
{
	class Blob;
	class BlobHolder;
	class Renderer : public Asset
	{
		friend class Effect;
	public:
		Renderer(const char* path);
		virtual ~Renderer();
	public:
		virtual void doLoad() override;
	public:
		std::shared_ptr<Blob> getInputSignatureBlob();
	private:
		bool createVertexShader();
		bool createPixelShader();
	private:
		ID3D11VertexShader*				m_vertexShader;
		ID3D11PixelShader*				m_pixelShader;
		std::shared_ptr<Blob>			m_inputSignatureBlob;
		std::shared_ptr<BlobHolder>		m_blobHolder;
	};

	inline std::shared_ptr<Blob> Renderer::getInputSignatureBlob()
	{
		return m_inputSignatureBlob;
	}
}