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
		static std::shared_ptr<Mesh> Create_Box_Position();
		static std::shared_ptr<Mesh> Create_Plane_PositionNormalTexcoord();
		static std::shared_ptr<Mesh> Create_Sphere_PositionNormalTexcoord();
		static std::shared_ptr<Mesh> Create_Triangle_PositionColor();
		static std::shared_ptr<Mesh> Create_Position3();
		static std::shared_ptr<Mesh> Create_FullScreenTriangle();
		static std::shared_ptr<Mesh> Create_Water(float width, float depth, unsigned int m, unsigned int n);
	private:
		static std::unordered_map<std::string, std::shared_ptr<Mesh>> m_cache;
	};
}