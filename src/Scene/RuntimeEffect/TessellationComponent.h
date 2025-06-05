#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class TessellationComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		TessellationComponent();
		virtual ~TessellationComponent() = default;
	public:
		GET(unsigned int, triEdgeTess0);
		GET(unsigned int, triEdgeTess1);
		GET(unsigned int, triEdgeTess2);
		GET(unsigned int, triInsideTess);

		void set_triEdgeTess0(unsigned int triEdgeTess0);
		void set_triEdgeTess1(unsigned int triEdgeTess1);
		void set_triEdgeTess2(unsigned int triEdgeTess2);
		void set_triInsideTess(unsigned int triInsideTess);
	private:
		void createMesh();
		unsigned int m_triEdgeTess0;
		unsigned int m_triEdgeTess1;
		unsigned int m_triEdgeTess2;
		unsigned int m_triInsideTess;
	};
}