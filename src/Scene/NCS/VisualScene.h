#pragma once
#include "Scene.h"

namespace Destiny
{
	class VisualScene : public Scene
	{
	public:
		VisualScene(const std::string& name);
		virtual ~VisualScene();
	public:
		virtual void initialize() override;
		virtual void uninitialize() override;
		virtual void update(float deltaTime) override;
		virtual void onCull() override;
	public:
		std::shared_ptr<Node> findCameraNode();
	private:
		std::shared_ptr<Node> m_cameraNode;
	};

}