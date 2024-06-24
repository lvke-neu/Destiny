#pragma once
#include "Engine/Asset.h"
#include <string>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D10Blob;
namespace Destiny
{
	class Blob;
	class BlobHolder;
	class RenderParameters;
	class Renderer : public Asset
	{
	public:
		Renderer(const char* path);
		virtual ~Renderer();
	public:
		virtual void doLoad() override;
	public:
		std::shared_ptr<Blob> getInputSignatureBlob();
		void setFloat(const char* name, float data);
		void fillRenderParameters(std::shared_ptr<RenderParameters> renderParameters);
	private:
		bool createVertexShader();
		bool createPixelShader();
	private:
		ID3D11VertexShader*				m_vertexShader;
		ID3D11PixelShader*				m_pixelShader;
		std::shared_ptr<Blob>			m_inputSignatureBlob;
		std::shared_ptr<BlobHolder>		m_blobHolder;
		ID3D10Blob*						m_vsCompiledBlob;
		ID3D10Blob*						m_psCompiledBlob;
	};

	inline std::shared_ptr<Blob> Renderer::getInputSignatureBlob()
	{
		return m_inputSignatureBlob;
	}
}