#pragma once
#include <memory>

namespace Destiny
{
	class GeoScene;
	class GeoSceneCuller
	{
	public:
		GeoSceneCuller(std::shared_ptr<GeoScene> geoScene);
	public:
		void onCull();
	private:
		std::shared_ptr<GeoScene> m_geoScene;
	};
}