#pragma once
#include "Visual3DComponent.h"

namespace Destiny
{
	class TerrainComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		TerrainComponent();
	public:
		GET_CLASS_NAME(TerrainComponent);
	private:
		void updateMesh();
	private:
		int m_width;
		int m_height;
		int m_m;
		int m_n;
	};
}