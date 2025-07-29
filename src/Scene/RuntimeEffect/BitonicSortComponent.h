#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class ComputerCommand;
	class Texture;
	class BitonicSortComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		BitonicSortComponent();
		virtual ~BitonicSortComponent() = default;
	public:
		GET(Button, test);
		void set_test(Button test);
	private:
		void generateFakeData();

		void indirectArgsCS();
		void preSortCS();

		void setCSConstants();
	private:
		unsigned int m_elementCount;
		std::vector<unsigned int> m_fakeSortData;
		std::vector<unsigned int> m_fakeIndexData;

		unsigned int m_cIndirectArgStride;
		std::shared_ptr<Texture> m_indirectArgsTexture;

		std::shared_ptr<Texture> m_sortTexture;
		std::shared_ptr<Texture> m_indexTexture;

		std::shared_ptr<ComputerCommand> m_indirectArgsCS;
		std::shared_ptr<ComputerCommand> m_preSortCS;

		Button m_test;
	};
}