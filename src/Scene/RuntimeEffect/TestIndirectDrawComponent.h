#pragma once
#include "Graphics/VisualComponent.h"

namespace Destiny
{
	class Mesh;
	class TestIndirectDrawComponent : public VisualComponent
	{
		RTTR_ENABLE(VisualComponent);
	public:
		TestIndirectDrawComponent();
		virtual ~TestIndirectDrawComponent() = default;
	private:
		std::shared_ptr<Mesh>	getIndirectMesh();
	};
}