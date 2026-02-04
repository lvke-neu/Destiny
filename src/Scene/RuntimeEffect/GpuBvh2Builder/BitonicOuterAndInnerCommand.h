#pragma once
#include "Graphics/GraphicsPipeline/GraphicsCommand.h"
#include <memory>

namespace Destiny
{
	class ComputerCommand;
	class Texture;
	class BitonicOuterAndInnerCommand : public GraphicsCommand
	{
	public:
		BitonicOuterAndInnerCommand();
		virtual ~BitonicOuterAndInnerCommand();
	public:
		virtual void execute(std::shared_ptr<GraphicsContext> deviceContext) override;
	public:
		void setParam
		(
			unsigned int cIndirectArgStride,
			unsigned int alignedNumElements,
			std::shared_ptr<ComputerCommand> outerSortCS,
			std::shared_ptr<ComputerCommand> innerSortCS,
			std::shared_ptr<Texture> indirectArgsTexture
		);
	private:
		unsigned int m_cIndirectArgStride;
		unsigned int m_alignedNumElements;
		std::shared_ptr<ComputerCommand> m_outerSortCS;
		std::shared_ptr<ComputerCommand> m_innerSortCS;
		std::shared_ptr<Texture> m_indirectArgsTexture;
	};
}