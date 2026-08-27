#pragma once
#include "GraphicsPipeline/GraphicsCommand.h"
#include <memory>

namespace Destiny
{
	class Texture;
	class CopyStructureCount : public GraphicsCommand
	{
	public:
		CopyStructureCount();
		virtual ~CopyStructureCount() = default;
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	public:
		void setDstAndSrcTexture(std::shared_ptr<Texture> dstTexture, std::shared_ptr<Texture> srcTexture);
	private:
		std::shared_ptr<Texture> m_dstTexture;
		std::shared_ptr<Texture> m_srcTexture;
	};
}