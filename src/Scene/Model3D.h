#pragma once
#include "Engine/Asset.h"
#include <vector>

namespace Destiny
{
	class Visual3DComponent;
	class Material;
	class Model3D : public Asset
	{
		friend class Model3DComponent;
	public:
		Model3D();
	public:
		virtual void doLoad() override;
	private:
		std::vector<std::shared_ptr<Visual3DComponent>> m_visual3DComponents;
		std::vector<std::shared_ptr<Material>> m_materials;
	};
}