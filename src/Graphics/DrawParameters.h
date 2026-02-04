#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

struct ID3D11Buffer;
struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
struct ID3D11BlendState;
namespace Destiny
{
	class  Blob;
	class  ConstantBuffer;
	class  Texture;
	struct TextureDesc;
	class  SamplerState;
	struct SamplerStateDesc;
	class DrawParameters
	{
	public:
		void* vertexBuffer									 = nullptr;
		unsigned int vertexBuffer_stride							 = 0;
		unsigned int vertexBuffer_offset							 = 0;
																	 
		void* instanceBuffer								 = nullptr;
		unsigned int instanceBuffer_stride							 = 0;
		unsigned int instanceBuffer_offset							 = 0;
		unsigned int instanceCount									 = 0;
																	 
		void* indexBuffer									 = nullptr;
		short format												 = 0;
		short primitiveTopology										 = 0;
		void* inputLayout								             = nullptr;
		std::shared_ptr<Blob> inputSignature;

		void* indirectBuffer								 = nullptr;
		std::vector<std::pair<short, unsigned int>>		drawIndirectMethod_indirectBufferOffsets;
																	 
		void* vertexShader							 = nullptr;
		void* pixelShader								 = nullptr;
		void* geometryShader						 = nullptr;
		void* hullShader						         = nullptr;
		void* domainShader						     = nullptr;
																	  
		void* rasterizerState						 = nullptr;
		void* depthStencilState					 = nullptr;
		void* blendState								 = nullptr;
																	 
		short drawType												 = 0;
		unsigned int indexCount										 = 0;
		unsigned int vertexCount									 = 0;
		std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>>												constantBuffers;
		std::unordered_map<std::string, std::pair<std::shared_ptr<TextureDesc>, std::shared_ptr<Texture>>>				textures;
		std::unordered_map<std::string, std::pair<std::shared_ptr<SamplerStateDesc>, std::shared_ptr<SamplerState>>>	samplerStates;
	};
}