#pragma once
#include "../NCS/Scene.h"

namespace Destiny
{
	class GeoScene : public Scene
	{
	public:
		GeoScene(const std::string& name);
		virtual ~GeoScene();
	public:
		virtual void initialize() override;
		virtual void uninitialize() override;
		virtual void update(float deltaTime) override;
		virtual void onCull() override;
	};

}