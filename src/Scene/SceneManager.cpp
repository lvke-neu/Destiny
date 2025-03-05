#include "SceneManager.h"
#include "Graphics/VisualScene.h"
#include "Graphics/CameraController.h"
#include "Graphics/DirectionLightComponent.h"
#include "Graphics/PointLightComponent.h"
#include "Graphics/PbrMaterial.h"
#include "Model/ModelComponent.h"
#include "RuntimeEffect/BoxComponent.h"
#include "RuntimeEffect/PlaneComponent.h"
#include "RuntimeEffect/TestGeometryShaderComponent.h"
#include "RuntimeEffect/PixelBillboardComponent.h"
#include "RuntimeEffect/SizeBillboardComponent.h"
#include "RuntimeEffect/InstancedComponet.h"
#include "RuntimeEffect/TextComponent.h"
#include "RuntimeEffect/SphereComponent.h"
#include "RuntimeEffect/ScriptComponent.h"
#include "RuntimeEffect/SkyboxComponent.h"
#include "RuntimeEffect/Particle/ParticleRainComponent.h"
#include "Math/Math.h"

namespace Destiny
{
	SceneManager::SceneManager() : 
		m_scene(std::make_shared<VisualScene>("VisualScene"))
	{
		
	}

	SceneManager::~SceneManager()
	{

	}	
	static std::shared_ptr<TextComponent> frameText;
	void SceneManager::initialize()
	{
		m_scene->initialize();

		//camera
		auto cameraTransfrom = m_scene->getCameraNode()->get_transform();
		cameraTransfrom.set_translation({ 0.764f, 7.887f, -12.614f });
		cameraTransfrom.set_rotation({ 18.880f, 0.0f, 1.0f });
		m_scene->getCameraNode()->set_transform(cameraTransfrom);
		m_scene->getCameraController()->set_speed(15.0f);

		//light 
		{
			{
				auto directionLightComponent = std::make_shared<DirectionLightComponent>();
				directionLightComponent->set_intensity(10.0f);
				auto directionLightNode = std::make_shared<Node>("DirectionLight");
				directionLightNode->addComponent(directionLightComponent);
				directionLightNode->addToParent(m_scene);
				Transform transform;
				transform.set_rotation({ 0.0f, -1.0f, 1.0f });
				directionLightNode->set_transform(transform);

				//auto lightGzimoComponent = std::make_shared<PixelBillboardComponent>();
				//lightGzimoComponent->set_size({ 50.0f, 50.0f });
				//lightGzimoComponent->set_texturePath("builtin://texture/directional_light_icon.png");
				//lightGzimoComponent->set_color({ 1.0f, 1.0f, 1.0f, 1.0f });
				//directionLightNode->addComponent(lightGzimoComponent);
			}

			//{
			//	auto pointLightComponent = std::make_shared<PointLightComponent>();
			//	pointLightComponent->set_intensity(100.0f);
			//	pointLightComponent->set_color({ 1.0f, 0.0f, 0.0f, 1.0f });
			//	auto pointLightNode = std::make_shared<Node>("PointLight X_Z");
			//	pointLightNode->addComponent(pointLightComponent);
			//	pointLightNode->addToParent(m_scene);
			//	Transform transform;
			//	//transform.set_translation({ -5.0f, 5.0f, 5.0f });
			//	pointLightNode->set_transform(transform);
			//	auto lightGzimoComponent = std::make_shared<PixelBillboardComponent>();
			//	lightGzimoComponent->set_size({ 50.0f, 50.0f });
			//	lightGzimoComponent->set_texturePath("builtin://texture/point_light_icon.png");
			//	lightGzimoComponent->set_color({ 1.0f, 0.0f, 0.0f, 1.0f });
			//	pointLightNode->addComponent(lightGzimoComponent);

			//	auto scriptComponent = std::make_shared<ScriptComponent>();
			//	scriptComponent->set_speed(5.0f);
			//	scriptComponent->set_length(10.0f);
			//	scriptComponent->set_circularMotion(CircularMotion::X_Z);
			//	pointLightNode->addComponent(scriptComponent);
			//}

			//{
			//	auto pointLightComponent = std::make_shared<PointLightComponent>();
			//	pointLightComponent->set_intensity(100.0f);
			//	pointLightComponent->set_color({ 0.0f, 1.0f, 0.0f, 1.0f });
			//	auto pointLightNode = std::make_shared<Node>("PointLight Y_Z");
			//	pointLightNode->addComponent(pointLightComponent);
			//	pointLightNode->addToParent(m_scene);
			//	Transform transform;
			//	//transform.set_translation({ -5.0f, 5.0f, 5.0f });
			//	pointLightNode->set_transform(transform);
			//	auto lightGzimoComponent = std::make_shared<PixelBillboardComponent>();
			//	lightGzimoComponent->set_size({ 50.0f, 50.0f });
			//	lightGzimoComponent->set_texturePath("builtin://texture/point_light_icon.png");
			//	lightGzimoComponent->set_color({ 0.0f, 1.0f, 0.0f, 1.0f });
			//	pointLightNode->addComponent(lightGzimoComponent);

			//	auto scriptComponent = std::make_shared<ScriptComponent>();
			//	scriptComponent->set_speed(5.0f);
			//	scriptComponent->set_length(10.0f);
			//	scriptComponent->set_circularMotion(CircularMotion::Y_Z);
			//	pointLightNode->addComponent(scriptComponent);
			//}

			//{
			//	auto pointLightComponent = std::make_shared<PointLightComponent>();
			//	pointLightComponent->set_intensity(100.0f);
			//	pointLightComponent->set_color({ 0.0f, 0.0f, 1.0f, 1.0f });
			//	auto pointLightNode = std::make_shared<Node>("PointLight X_Y");
			//	pointLightNode->addComponent(pointLightComponent);
			//	pointLightNode->addToParent(m_scene);
			//	Transform transform;
			//	//transform.set_translation({ -5.0f, 5.0f, 5.0f });
			//	pointLightNode->set_transform(transform);
			//	auto lightGzimoComponent = std::make_shared<PixelBillboardComponent>();
			//	lightGzimoComponent->set_size({ 50.0f, 50.0f });
			//	lightGzimoComponent->set_texturePath("builtin://texture/point_light_icon.png");
			//	lightGzimoComponent->set_color({ 0.0f, 0.0f, 1.0f, 1.0f });
			//	pointLightNode->addComponent(lightGzimoComponent);

			//	auto scriptComponent = std::make_shared<ScriptComponent>();
			//	scriptComponent->set_speed(5.0f);
			//	scriptComponent->set_length(10.0f);
			//	scriptComponent->set_circularMotion(CircularMotion::X_Y);
			//	pointLightNode->addComponent(scriptComponent);
			//}

			//auto pointLightNode = std::make_shared<Node>("PointLight");
			//pointLightNode->addToParent(m_scene);
			//for (int i = -500; i < 500; i++)
			//{
			//	auto pointLightComponent = std::make_shared<PointLightComponent>();
			//	pointLightComponent->set_intensity(100.0f);
			//	pointLightComponent->set_color({ Math::RandomFloat(0.0f, 1.0f), Math::RandomFloat(0.0f, 1.0f), Math::RandomFloat(0.0f, 1.0f), 1.0f });
			//	
			//	auto node = std::make_shared<Node>("PointLight" + std::to_string(i));
			//	node->addToParent(pointLightNode);
			//	node->addComponent(pointLightComponent);
			//	
			//	Transform transform;
			//	transform.set_translation({ (float)i, 1.0f, 0.0f });
			//	node->set_transform(transform);
			//}

		}

		//plane
		{
			auto planeComponent = std::make_shared<PlaneComponent>();
			planeComponent->set_material(PbrMaterial::Create_Peel());

			auto node = std::make_shared<Node>();
			node->set_name("PlaneNode");
			node->addComponent(planeComponent);
			node->addToParent(m_scene);

			Transform transform;
			transform.set_scale({ 50.0f, 20.0f, 1.0f });
			transform.set_rotation({ 90.0f, 0.0f, 0.0f });
			transform.set_translation({ 0.0f, 0.0f, 0.0f });
			node->set_transform(transform);
		}
		//sphere
		{
			//auto sphereNode = std::make_shared<Node>();
			//sphereNode->set_name("SphereNode");
			//sphereNode->addToParent(m_scene);

			//{
			//	auto sphereComponent = std::make_shared<SphereComponent>();
			//	sphereComponent->set_material(PbrMaterial::Create_Default());

			//	auto node = std::make_shared<Node>();
			//	node->set_name("SphereNode1");
			//	node->addComponent(sphereComponent);
			//	node->addToParent(sphereNode);

			//	Transform transform;
			//	transform.set_translation({ -4.0f, 3.0f, 0.0f });
			//	node->set_transform(transform);
			//}

			//{
			//	auto sphereComponent = std::make_shared<SphereComponent>();
			//	sphereComponent->set_material(PbrMaterial::Create_Gold());

			//	auto node = std::make_shared<Node>();
			//	node->set_name("SphereNode2");
			//	node->addComponent(sphereComponent);
			//	node->addToParent(sphereNode);

			//	Transform transform;
			//	transform.set_translation({ -2.0f, 3.0f, 0.0f });
			//	node->set_transform(transform);
			//}

			//{
			//	auto sphereComponent = std::make_shared<SphereComponent>();
			//	sphereComponent->set_material(PbrMaterial::Create_Grass());

			//	auto node = std::make_shared<Node>();
			//	node->set_name("SphereNode3");
			//	node->addComponent(sphereComponent);
			//	node->addToParent(sphereNode);

			//	Transform transform;
			//	transform.set_translation({ 0.0f, 3.0f, 0.0f });
			//	node->set_transform(transform);
			//}

			//{
			//	auto sphereComponent = std::make_shared<SphereComponent>();
			//	sphereComponent->set_material(PbrMaterial::Create_Plastic());

			//	auto node = std::make_shared<Node>();
			//	node->set_name("SphereNode4");
			//	node->addComponent(sphereComponent);
			//	node->addToParent(sphereNode);

			//	Transform transform;
			//	transform.set_translation({ 2.0f, 3.0f, 0.0f });
			//	node->set_transform(transform);
			//}

			//{
			//	auto sphereComponent = std::make_shared<SphereComponent>();
			//	sphereComponent->set_material(PbrMaterial::Create_Rusted_Iron());

			//	auto node = std::make_shared<Node>();
			//	node->set_name("SphereNode5");
			//	node->addComponent(sphereComponent);
			//	node->addToParent(sphereNode);

			//	Transform transform;
			//	transform.set_translation({ 4.0f, 3.0f, 0.0f });
			//	node->set_transform(transform);
			//}

			//{
			//	auto sphereComponent = std::make_shared<SphereComponent>();
			//	sphereComponent->set_material(PbrMaterial::Create_Wall());

			//	auto node = std::make_shared<Node>();
			//	node->set_name("SphereNode6");
			//	node->addComponent(sphereComponent);
			//	node->addToParent(sphereNode);

			//	Transform transform;
			//	transform.set_translation({ 6.0f, 3.0f, 0.0f });
			//	node->set_transform(transform);
			//}
		}
		////model
		//{
		//	{
		//		auto modelComponent = std::make_shared<ModelComponent>();
		//		modelComponent->set_path("builtin://model/nanosuit/nanosuit.obj");
		//		auto node = std::make_shared<Node>();
		//		Transform transform;
		//		transform.set_scale({ 0.11f, 0.11f, 0.11f });
		//		transform.set_rotation({ 0.0f, 0.0f, 0.0f });
		//		transform.set_translation({ 2.0f, 0.0f, 0.0f });
		//		node->set_name("nanosuit");
		//		node->addComponent(modelComponent);
		//		node->addToParent(m_scene);
		//		node->set_transform(transform);
		//	}
		//	{
		//		auto modelComponent = std::make_shared<ModelComponent>();
		//		modelComponent->set_path("builtin://model/walk/Standard Walk.dae");
		//		auto node = std::make_shared<Node>();
		//		node->set_name("Walk");
		//		node->addComponent(modelComponent);
		//		node->addToParent(m_scene);
		//	}
		//	{
		//		auto modelComponent = std::make_shared<ModelComponent>();
		//		modelComponent->set_path("builtin://model/Jumping/Jumping.dae");
		//		auto node = std::make_shared<Node>();
		//		Transform transform;
		//		transform.set_translation({ -2.0f, 0.0f, 0.0f });
		//		node->set_name("Jump");
		//		node->addComponent(modelComponent);
		//		node->addToParent(m_scene);
		//		node->set_transform(transform);
		//	}
		//	{
		//		auto modelComponent = std::make_shared<ModelComponent>();
		//		modelComponent->set_path("builtin://model/planet/planet.obj");
		//		auto node = std::make_shared<Node>();
		//		Transform transform;
		//		transform.set_translation({ 33.0f, 11.0f, 55.0f });
		//		transform.set_scale({ 2.0f, 2.0f, 2.0f });
		//		node->set_name("planet");
		//		node->addComponent(modelComponent);
		//		node->addToParent(m_scene);
		//		node->set_transform(transform);
		//	}

		//	{
		//		auto modelComponent = std::make_shared<ModelComponent>();
		//		modelComponent->set_path("builtin://model/chinese_tea_table_2k/chinese_tea_table_2k.gltf");
		//		auto node = std::make_shared<Node>();
		//		Transform transform;
		//		transform.set_translation({ -10.0f, 0.0f, 10.0f });
		//		transform.set_scale({ 5.0f, 5.0f, 5.0f });
		//		node->set_name("tea_table");
		//		node->addComponent(modelComponent);
		//		node->addToParent(m_scene);
		//		node->set_transform(transform);
		//	}

		//	{
		//		auto modelComponent = std::make_shared<ModelComponent>();
		//		modelComponent->set_path("builtin://model/chinese_armchair_2k/chinese_armchair_2k.gltf");
		//		auto node = std::make_shared<Node>();
		//		Transform transform;
		//		transform.set_translation({ 10.0f, 0.0f, 10.0f });
		//		transform.set_scale({ 5.0f, 5.0f, 5.0f });
		//		node->set_name("armchair");
		//		node->addComponent(modelComponent);
		//		node->addToParent(m_scene);
		//		node->set_transform(transform);
		//	}

		//	{
		//		auto modelComponent = std::make_shared<ModelComponent>();
		//		modelComponent->set_path("builtin://model/CoffeeCart_01_2k/CoffeeCart_01_2k.gltf");
		//		auto node = std::make_shared<Node>();
		//		Transform transform;
		//		transform.set_translation({ 0.0f, 0.0f, 10.0f });
		//		transform.set_scale({ 5.0f, 5.0f, 5.0f });
		//		node->set_name("CoffeeCart");
		//		node->addComponent(modelComponent);
		//		node->addToParent(m_scene);
		//		node->set_transform(transform);
		//	}

		//	{
		//		auto modelComponent = std::make_shared<ModelComponent>();
		//		modelComponent->set_path("builtin://model/korean_fire_extinguisher_01_2k/korean_fire_extinguisher_01_2k.gltf");
		//		auto node = std::make_shared<Node>();
		//		Transform transform;
		//		transform.set_translation({ 0.0f, 0.0f, 5.0f });
		//		transform.set_scale({ 5.0f, 5.0f, 5.0f });
		//		node->set_name("fire_extinguisher");
		//		node->addComponent(modelComponent);
		//		node->addToParent(m_scene);
		//		node->set_transform(transform);
		//	}

		//	{
		//		auto modelComponent = std::make_shared<ModelComponent>();
		//		modelComponent->set_path("builtin://model/food_apple_01_2k/food_apple_01_2k.gltf");
		//		auto node = std::make_shared<Node>();
		//		Transform transform;
		//		transform.set_translation({ -6.0f, 3.0f, 0.0f });
		//		transform.set_scale({ 10.0f, 10.0f, 10.0f });
		//		node->set_name("Apple");
		//		node->addComponent(modelComponent);
		//		node->addToParent(m_scene);
		//		node->set_transform(transform);
		//	}

		//	{
		//		auto modelComponent = std::make_shared<ModelComponent>();
		//		modelComponent->set_path("builtin://model/garden_gnome_2k/garden_gnome_2k.gltf");
		//		auto node = std::make_shared<Node>();
		//		Transform transform;
	
		//		transform.set_translation({ -4.0f, 0.0f, 0.0f });
		//		transform.set_scale({ 3.0f, 3.0f, 3.0f });
		//		node->set_name("garden_gnome");
		//		node->addComponent(modelComponent);
		//		node->addToParent(m_scene);
		//		node->set_transform(transform);
		//	}
		//}	

		//particle
		//{
		//	auto particleRainComponent = std::make_shared<ParticleRainComponent>();
		//	auto node = std::make_shared<Node>();
		//	node->set_name("ParticleRain");
		//	node->addToParent(m_scene);
		//	node->addComponent(particleRainComponent);
		//}
		//text
		//{
		//	auto textComponent = std::make_shared<TextComponent>();
		//	textComponent->set_text("l v k e");
		//	textComponent->set_size({ 150.0f, 50.0f });
		//	textComponent->set_screenPosition({ -0.8f, 0.9f });
		//	textComponent->set_color({ 1.0f, 1.0f, 0, 1.0f });

		//	frameText = std::make_shared<TextComponent>();
		//	frameText->set_size({ 400.0f, 50.0f });
		//	frameText->set_screenPosition({ 0.0f, 0.8f });
		//	frameText->set_color({ 0.0f, 1.0f, 0.0f, 1.0f });

		//	auto node = std::make_shared<Node>();
		//	node->set_name("Text");
		//	node->addToParent(m_scene);
		//	node->addComponent(textComponent);
		//	node->addComponent(frameText);

		//}
		
		//skybox
		//{
		//	auto skyboxComponent = std::make_shared<SkyboxComponent>();
		//	skyboxComponent->set_enable(true);
		//	auto node = std::make_shared<Node>();
		//	node->set_name("Skybox");
		//	node->addComponent(skyboxComponent);
		//	node->addToParent(m_scene);
		//}

	}

	void SceneManager::uninitialize()
	{
		m_scene->uninitialize();
	}

	void SceneManager::update(float deltaTime)
	{
		m_scene->update(deltaTime);
		m_scene->onCull();

		//static unsigned int framCount = 0;
		//framCount ++;

		//static float sumTime = 0.0f;
		//sumTime += deltaTime;

		//if (sumTime > 1.0f)
		//{
		//	auto timeStr = "F r a m e C o u n t : " + std::to_string(framCount);
		//	frameText->set_text(timeStr);
		//	sumTime = 0.0f;
		//}
	}
}