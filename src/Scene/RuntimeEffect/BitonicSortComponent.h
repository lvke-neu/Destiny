#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class Texture;
	class BitonicSortComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		BitonicSortComponent();
		virtual ~BitonicSortComponent() = default;
	private:
		void indirectArgsCS();
	private:
		unsigned int m_cIndirectArgStride;
		std::shared_ptr<Texture> m_indirectArgsTexture;
	};
}