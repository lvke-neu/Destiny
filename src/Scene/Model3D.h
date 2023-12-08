#pragma once
#include "Engine/Asset.h"
#include <vector>

namespace Destiny
{
	class Model3DComponent;
	class Visual3DComponent;
	class Model3D : public Asset
	{
		friend class Model3DComponent;
	public:
		Model3D(std::shared_ptr<Model3DComponent> model3DComponent);
		virtual ~Model3D();
	public:
		virtual void doLoad() override;
	private:
		std::vector<std::shared_ptr<Visual3DComponent>> m_visual3DComponents;
		std::shared_ptr<Model3DComponent> m_model3DComponent;
	};
}