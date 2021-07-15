#include "urpch.h"
#include "Scene.h"
#include "Components.h"

#include "Uranus/Renderer/Renderer2D.h"

#include "Entity.h"

#include <glm/glm.hpp>

namespace Uranus {

	Scene::Scene() {
		
	}

	Scene::~Scene() {

	}

	void Scene::OnUpdate(Timestep ts)
	{

		// Update And Create Scripts
		_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			if(!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->_Entity = Entity{ entity,  this };
				nsc.Instance->OnCreate();
			}

			nsc.Instance->OnUpdate(ts);
		});

		// Rendere
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		auto view = _Registry.view<CameraComponent, TransformComponent>();

		for (auto entity : view) {
			
			auto& [camera, transform] = view.get<CameraComponent, TransformComponent>(entity);
			if (camera.Primary) {
				mainCamera = &camera.Camera;
				cameraTransform = transform.GetTransform();
				break;
			}
		}

		if (mainCamera) {
			Uranus::Renderer2D::BeginScene(*mainCamera, cameraTransform);
	
			auto group = _Registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group) {
				auto& [transfrom, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Uranus::Renderer2D::DrawQuad(transfrom.GetTransform(), sprite.Color);
			}

			Uranus::Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		_ViewPortWidth = width;
		_ViewPortHeight = height;

		auto view = _Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { _Registry.create() , this };
		entity.AddComponent<TransformComponent>();
		auto& tagComponent = entity.AddComponent<TagComponent>();
		tagComponent.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DeleteEntity(Entity entity)
	{
		_Registry.destroy(entity);
	}

	template<typename T>
	void Scene::OnComponenetAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponenetAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(_ViewPortWidth, _ViewPortHeight);
	}

	template<>
	void Scene::OnComponenetAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponenetAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponenetAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponenetAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
}