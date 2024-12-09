#pragma once
#include <string>
#include <memory>
#include <unordered_map>

namespace Destiny
{
	class Mesh;
	class MeshProvider
	{
	public:
		static std::shared_ptr<Mesh> Create_Box_PositionNormalTexcoord();
		static std::shared_ptr<Mesh> Create_Plane_PositionNormalTexcoord();
		static std::shared_ptr<Mesh> Create_Triangle_PositionColor();
	private:
		static std::unordered_map<std::string, std::shared_ptr<Mesh>> m_cache;
	};
}