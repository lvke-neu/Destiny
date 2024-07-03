#pragma once
#include <string>
#include <memory>
#include <unordered_map>

struct ID3D11Buffer;
struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
struct ID3D11BlendState;
namespace Destiny
{
	class  ConstantBuffer;
	class  Texture;
	struct TextureDesc;
	class  SamplerState;
	struct SamplerStateDesc;
	class RenderParameters
	{
	public:
		ID3D11Buffer* vertexBuffer						= nullptr;
		unsigned int stride								= 0;
		unsigned int offset								= 0;
		ID3D11Buffer* indexBuffer						= nullptr;
		short format									= 0;
		short primitiveTopology							= 0;
		ID3D11InputLayout* inputLayout					= nullptr;

		ID3D11VertexShader* vertexShader				= nullptr;
		ID3D11PixelShader* pixelShader					= nullptr;

		ID3D11RasterizerState* rasterizerState			= nullptr;
		ID3D11DepthStencilState* depthStencilState		= nullptr;
		ID3D11BlendState* blendState					= nullptr;

		short drawType									= 0;
		unsigned int indexCount							= 0;
		unsigned int vertexCount						= 0;
		std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>>	constantBuffers;
		std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>>	textures;
		std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>> samplerStates;
	};
}