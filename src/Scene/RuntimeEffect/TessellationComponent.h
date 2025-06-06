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
		GET(float, maxTessellation);
		GET(float, minTessellation);
		GET(float, tessellationRange);
		GET(float, heightScale);

		void set_maxTessellation(float maxTessellation);
		void set_minTessellation(float minTessellation);
		void set_tessellationRange(float tessellationRange);
		void set_heightScale(float heightScale);
	private:
		void createMesh();
		float m_maxTessellation;
		float m_minTessellation;
		float m_tessellationRange;
		float m_heightScale;
	};
}