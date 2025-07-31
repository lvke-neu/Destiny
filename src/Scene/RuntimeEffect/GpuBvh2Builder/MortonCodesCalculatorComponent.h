#pragma once
#include "Engine/Component.h"

namespace Destiny
{
	class ComputerCommand;
	class GraphicsCommandList;
	class Texture;
	class MortonCodesCalculatorComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		MortonCodesCalculatorComponent();
		virtual ~MortonCodesCalculatorComponent() = default;
	private:
		void calcuateMortonCodesForAABBsCS();
	private:
		std::shared_ptr<ComputerCommand> m_calcuateMortonCodesForAABBs;

		std::shared_ptr<GraphicsCommandList>	m_graphicsCommandList;
	};
}