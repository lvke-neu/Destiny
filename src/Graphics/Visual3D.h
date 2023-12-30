#pragma once
#include <memory>
#include <functional>
#include <vector>
#include <d3d11.h>

namespace Destiny
{
	class VertexBuffer;
	class IndexBuffer;
	class InputLayout;
	class VertexShader;
	class PixelShader;
	class GeometryShader;
	class RasterizerState;
	class DepthStencilState;
	class BlendState;
	class Visual3D
	{
	public:
		enum RenderToMask
		{
			render_to_scene = 1,
			render_to_texture = 2,
			render_to_shadow_map = 4
		};
	public:
		Visual3D();
		~Visual3D();
	public:
		void draw();
		void registerBeforeDrawCommands(std::function<void(void)> beforeDrawCommand)
		{
			m_beforeDrawCommands.push_back(beforeDrawCommand);
		}
		void registerAfterDrawCommands(std::function<void(void)> afterDrawCommand)
		{
			m_afterDrawCommands.push_back(afterDrawCommand);
		}
	public:
		void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
		void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);
		void setInputLayout(std::shared_ptr<InputLayout> inputLayout);
		void setVertexShader(std::shared_ptr<VertexShader> vertexShader);
		std::shared_ptr<PixelShader> getPixelShader();
		void setPixelShader(std::shared_ptr<PixelShader> pixelShader);
		std::shared_ptr<GeometryShader> getGeometryShader();
		void setGeometryShader(std::shared_ptr<GeometryShader> geometryShader);
		void setRasterizerState(std::shared_ptr<RasterizerState> rasterizerState);
		void setDepthStencilState(std::shared_ptr<DepthStencilState> depthStencilState);
		void setBlendState(std::shared_ptr<BlendState> blendState);
		void setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitiveTopology);
		void setRenderToMask(RenderToMask mask);
	private:
		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		std::shared_ptr<InputLayout> m_inputLayout;
		std::shared_ptr<VertexShader> m_vertexShader;
		std::shared_ptr<PixelShader> m_pixelShader;
		std::shared_ptr<GeometryShader> m_geometryShader;
		std::shared_ptr<RasterizerState> m_rasterizerState;
		std::shared_ptr<DepthStencilState> m_depthStencilState;
		std::shared_ptr<BlendState> m_blendState;

		std::vector<std::function<void(void)>> m_beforeDrawCommands;
		std::vector < std::function<void(void)>> m_afterDrawCommands;
		D3D_PRIMITIVE_TOPOLOGY m_primitiveTopology;
		RenderToMask m_renderToMask;
	};

	inline void Visual3D::setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		if (m_vertexBuffer)
		{
			m_vertexBuffer.reset();
		}
		m_vertexBuffer = vertexBuffer;
	}

	inline void Visual3D::setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
	{
		if (m_indexBuffer)
		{
			m_indexBuffer.reset();
		}
		m_indexBuffer = indexBuffer;
	}

	inline void Visual3D::setInputLayout(std::shared_ptr<InputLayout> inputLayout)
	{
		if (m_inputLayout)
		{
			m_inputLayout.reset();
		}
		m_inputLayout = inputLayout;
	}

	inline void Visual3D::setVertexShader(std::shared_ptr<VertexShader> vertexShader)
	{
		if (m_vertexShader)
		{
			m_vertexShader.reset();
		}
		m_vertexShader = vertexShader;
	}

	inline std::shared_ptr<PixelShader> Visual3D::getPixelShader()
	{
		return m_pixelShader;
	}

	inline void Visual3D::setPixelShader(std::shared_ptr<PixelShader> pixelShader)
	{
		if (m_pixelShader)
		{
			m_pixelShader.reset();
		}
		m_pixelShader = pixelShader;
	}

	inline  std::shared_ptr<GeometryShader> Visual3D::getGeometryShader()
	{
		return m_geometryShader;
	}

	inline void Visual3D::setGeometryShader(std::shared_ptr<GeometryShader> geometryShader)
	{
		if (m_geometryShader)
		{
			m_geometryShader.reset();
		}
		m_geometryShader = geometryShader;
	}

	inline void Visual3D::setRasterizerState(std::shared_ptr<RasterizerState> rasterizerState)
	{
		if (m_rasterizerState)
		{
			m_rasterizerState.reset();
		}
		m_rasterizerState = rasterizerState;
	}

	inline void Visual3D::setDepthStencilState(std::shared_ptr<DepthStencilState> depthStencilState)
	{
		if (m_depthStencilState)
		{
			m_depthStencilState.reset();
		}
		m_depthStencilState = depthStencilState;
	}

	inline void Visual3D::setBlendState(std::shared_ptr<BlendState> blendState)
	{
		if (m_blendState)
		{
			m_blendState.reset();
		}
		m_blendState = blendState;
	}

	inline void Visual3D::setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitiveTopology)
	{
		m_primitiveTopology = primitiveTopology;
	}

	inline void Visual3D::setRenderToMask(RenderToMask mask)
	{
		m_renderToMask = mask;
	}
}