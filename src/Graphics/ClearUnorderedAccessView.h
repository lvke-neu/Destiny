#pragma once
#include "GraphicsPipeline/GraphicsCommand.h"
#include <memory>

namespace Destiny
{
	class Texture;
	class ClearUnorderedAccessView : public GraphicsCommand
	{
	public:
		enum ClearType
		{
			None = 0,
			ClearFloat,
			ClearUint
		};
	public:
		ClearUnorderedAccessView();
		virtual ~ClearUnorderedAccessView();
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	public:
		void setParam(ClearType clearType, std::shared_ptr<Texture> texture);
	private:
		ClearType m_clearType;
		std::shared_ptr<Texture> m_texture;
	};
}