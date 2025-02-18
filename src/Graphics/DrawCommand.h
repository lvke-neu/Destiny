#pragma once
#include "GraphicsPipeline/GraphicsCommand.h"
#include <memory>
#include <unordered_set>

namespace Destiny
{
	class DrawParameters;
	class DrawCommand : public GraphicsCommand
	{
	public:
		virtual ~DrawCommand() = default;
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	public:
		void addDrawParameter(std::shared_ptr<DrawParameters> drawParameters);
		void clearDrawParameter();
	private:
		std::unordered_set<std::shared_ptr<DrawParameters>> m_drawParameters;
	};
}