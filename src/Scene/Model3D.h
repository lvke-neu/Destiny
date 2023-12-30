#pragma once
#include "Engine/Asset.h"
#include <vector>
#include <string>

namespace Destiny
{
	class Visual3DComponent;
	class Model3D : public Asset
	{
		friend class Model3DComponent;
	public:
		virtual void doLoad() override;
	private:
		std::vector<std::pair<std::string, std::shared_ptr<Visual3DComponent>>> m_visual3DComponents;
	};
}