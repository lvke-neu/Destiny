#include "Scene3D.h"
#include "Node3D.h"
#include "Component.h"
#include "Visual3DComponent.h"
#include "BoxComponent.h"
#include "SphereComponent.h"
#include "Camera3DComponent.h"
#include "CameraController.h"
#include "DirectLightComponent.h"
#include "PointLightComponent.h"
#include "PlaneComponent.h"
#include "Model3DComponent.h"
#include "BillboardComponent.h"
#include "SkyboxComponent.h"
#include "RenderToTextureComponent.h"
#include "ParticleComponent.h"
#include "TerrainComponent.h"
#include "PBRSphereComponent.h"
#include "Graphics/Visual3D.h"

namespace Destiny
{
	Scene3D::Scene3D() 
	{
		
	}

	void Scene3D::initialize()
	{
		m_rootNode = std::make_shared<Node3D>(shared_from_this());
		m_rootNode->set_name("Root");


		m_cameraNode = std::make_shared<Node3D>();
		m_cameraNode->set_name("Camera");
		m_cameraNode->addToParent(m_rootNode);
		Transform3D transform3D;
		transform3D.set_translation({ 0.0f, 5.0f, -20.0f });
		//transform3D.set_rotation({ 50.0f, 0.0f, 0.0f });
		m_cameraNode->set_transform3D(transform3D);
		auto camera3DComponent = std::make_shared<Camera3DComponent>();
		m_cameraNode->addComponent(camera3DComponent);
		auto cameraController = std::make_shared<CameraController>();
		m_cameraNode->addComponent(cameraController);

		auto directLightNode = std::make_shared<Node3D>();
		directLightNode->set_name("DirectLight");
		directLightNode->addToParent(m_rootNode);
		transform3D = Transform3D();
		transform3D.set_translation({ 0.0f, 5.0f, -5.0f });
		transform3D.set_rotation({ 50.0f, 0.0f, 0.0f });
		directLightNode->set_transform3D(transform3D);
		auto directLightComponent = std::make_shared<DirectLightComponent>();
		directLightNode->addComponent(directLightComponent); 

		auto pointLightNode = std::make_shared<Node3D>();
		pointLightNode->set_name("PointLight");
		pointLightNode->addToParent(m_rootNode);
		transform3D = Transform3D();
		transform3D.set_translation({ 8.0f, 9.0f, -10.0f });
		transform3D.set_scale({ 0.2f, 0.2f, 0.2f });
		pointLightNode->set_transform3D(transform3D);
		auto pointLightComponent = std::make_shared<PointLightComponent>();
		pointLightNode->addComponent(pointLightComponent);

		auto skyboxNode = std::make_shared<Node3D>();
		skyboxNode->set_name("Skybox");
		skyboxNode->addToParent(m_rootNode);
		auto skyboxComponent = std::make_shared<SkyboxComponent>();
		skyboxNode->addComponent(skyboxComponent);

		auto planeNode = std::make_shared<Node3D>();
		planeNode->set_name("Plane");
		planeNode->addToParent(m_rootNode);
		transform3D = Transform3D();
		transform3D.set_translation({ -20.0f, 0.0f, 0.0f });
		planeNode->set_transform3D(transform3D);
		auto planeComponent = std::make_shared<PlaneComponent>();
		planeNode->addComponent(planeComponent);


		auto pbrNode = std::make_shared<Node3D>();
		pbrNode->set_name("PBR");
		pbrNode->addToParent(m_rootNode);
		transform3D = Transform3D();
		const int count = 16;
		for (int x = 0; x <= count; x += 4)
		{	
			for (int y = count; y >= 0; y -= 4)
			{
				auto tmpNode = std::make_shared<Node3D>();
				tmpNode->addToParent(pbrNode);
				transform3D.set_translation({ (float)x, (float)y, -5.0f });
				tmpNode->set_transform3D(transform3D);
				auto pbrSphereComponent = std::make_shared<PBRSphereComponent>();
				tmpNode->addComponent(pbrSphereComponent);
				auto roughness = x / (float)count;
				if (roughness == 0.0f)
				{
					roughness = 0.05f;
				}
		
				pbrSphereComponent->set_metallic(y / (float)count);
				pbrSphereComponent->set_roughness(roughness);	
			}
		}


		//auto terrainNode = std::make_shared<Node3D>();
		//terrainNode->set_name("Terrain");
		//terrainNode->addToParent(m_rootNode);
		//transform3D = Transform3D();
		//transform3D.set_translation({ 0.0f, -10.0f, 0.0f });
		////transform3D.set_scale({ 50.0f, 50.0f, 1.0f });
		////transform3D.set_rotation({ 90.0f, 0.0f, 0.0f });
		//terrainNode->set_transform3D(transform3D);
		//auto terrainComponent = std::make_shared<TerrainComponent>();
		//terrainNode->addComponent(terrainComponent);

		//auto boxNode = std::make_shared<Node3D>();
		//transform3D = Transform3D();
		//boxNode->set_name("Box");
		//boxNode->addToParent(m_rootNode);
		//auto boxComponent = std::make_shared<BoxComponent>();
		//boxNode->addComponent(boxComponent);
		//transform3D.set_translation({ -3.0f, 0.0f, 0.0f });
		//boxNode->set_transform3D(transform3D);
		//boxComponent->get_visual3D()->setRenderToMask(Visual3D::RenderToMask::render_to_scene | Visual3D::RenderToMask::render_to_shadow_map);

		//auto sphereNode = std::make_shared<Node3D>();
		//sphereNode->set_name("Sphere");
		//sphereNode->addToParent(m_rootNode);
		//transform3D = Transform3D();
		//transform3D.set_translation({ 3.0f, 0.0f, 0.0f });
		//sphereNode->set_transform3D(transform3D);
		//auto sphereComponent = std::make_shared<SphereComponent>();
		//sphereNode->addComponent(sphereComponent);
		//sphereComponent->get_visual3D()->setRenderToMask(Visual3D::RenderToMask::render_to_scene | Visual3D::RenderToMask::render_to_shadow_map);


		//auto modelNode = std::make_shared<Node3D>();
		//modelNode->set_name("Model");
		//modelNode->addToParent(m_rootNode);
		//transform3D = Transform3D();

		//auto walkNode = std::make_shared<Node3D>();
		//walkNode->addToParent(modelNode);
		//walkNode->set_name("Walk");
		//transform3D.set_scale({ 0.05f, 0.05f, 0.05f });
		//transform3D.set_translation({ -5.0f, 0.0f, 0.0f });
		//walkNode->set_transform3D(transform3D);
		//auto walkComponent = std::make_shared<Model3DComponent>();
		//walkNode->addComponent(walkComponent);
		//walkComponent->set_path("assets://Model/walk/Standard Walk.dae");
		//walkComponent->set_renderToMask(Visual3D::RenderToMask::render_to_scene | Visual3D::RenderToMask::render_to_shadow_map);

		//auto ymcaNode = std::make_shared<Node3D>();
		//ymcaNode->addToParent(modelNode);
		//ymcaNode->set_name("Ymca");
		//transform3D.set_translation({ 5.0f, 0.0f, 0.0f });
		//ymcaNode->set_transform3D(transform3D);
		//auto ymcaComponent = std::make_shared<Model3DComponent>();
		//ymcaNode->addComponent(ymcaComponent);
		//ymcaComponent->set_path("assets://Model/Ymca Dance/Ymca Dance.dae");
		//ymcaComponent->set_renderToMask(Visual3D::RenderToMask::render_to_scene | Visual3D::RenderToMask::render_to_shadow_map);

		//auto renderToTextureNode = std::make_shared<Node3D>();
		//renderToTextureNode->set_name("RenderToTexture");
		//renderToTextureNode->addToParent(m_rootNode);
		//auto renderToTextureComponent = std::make_shared<RenderToTextureComponent>();
		//renderToTextureNode->addComponent(renderToTextureComponent);

		//auto particleNode = std::make_shared<Node3D>();
		//particleNode->set_name("Particle");
		//particleNode->addToParent(m_rootNode);
		//for (int i = -10; i < 10; i++)
		//{
		//	auto tmpParticleNode = std::make_shared<Node3D>();
		//	auto particleComponent = std::make_shared<ParticleComponent>();
		//	particleComponent->m_initY = (float)(rand() % 20) + 10;
		//	transform3D = Transform3D();
		//	transform3D.set_translation({ (float)i, particleComponent->m_initY, 0.0f });
		//	tmpParticleNode->set_transform3D(transform3D);
		//	tmpParticleNode->addComponent(particleComponent);
		//	tmpParticleNode->addToParent(particleNode);
		//}
	}
}