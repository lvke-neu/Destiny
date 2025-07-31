#pragma once
#include "Engine/Component.h"
#include "Graphics/GraphicsDefine.h"
#include <vector>

namespace Destiny
{
	class ComputerCommand;
	class Texture;
	class BitonicOuterAndInnerCommand;
	class GraphicsCommandList;
	class BitonicSortComponent : public Component
	{
		RTTR_ENABLE(Component);
	public:
		BitonicSortComponent();
		virtual ~BitonicSortComponent() = default;
	public:
		GET(Button, test);
		void set_test(Button test);
		GET(unsigned int, elementCount);
		void set_elementCount(unsigned int elementCount);
	private:
		void generateFakeData();

		void indirectArgsCS();
		void preSortCS();
		void outerSortCS();
		void innerSortCS();
		void bitonicOuterAndInnerCommand();

		void setCSConstants();
	private:
		unsigned int m_elementCount;
		std::vector<unsigned int> m_fakeSortData;
		std::vector<unsigned int> m_fakeIndexData;

		unsigned int m_cIndirectArgStride;
		unsigned int m_alignedNumElements;
		std::shared_ptr<Texture> m_indirectArgsTexture;

		std::shared_ptr<Texture> m_sortTexture;
		std::shared_ptr<Texture> m_indexTexture;

		std::shared_ptr<ComputerCommand> m_indirectArgsCS;
		std::shared_ptr<ComputerCommand> m_preSortCS;
		std::shared_ptr<ComputerCommand> m_outerSortCS;
		std::shared_ptr<ComputerCommand> m_innerSortCS;
		std::shared_ptr<BitonicOuterAndInnerCommand> m_bitonicOuterAndInnerCommand;
		std::shared_ptr<GraphicsCommandList>	m_graphicsCommandList;

		Button m_test;
	};
}