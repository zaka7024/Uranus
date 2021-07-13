#include "urpch.h"
#include "Scene.h"
#include "Components.h"

#include "Uranus/Renderer/Renderer2D.h"

#include "Entity.h"

#include <glm/glm.hpp>

namespace Uranus {

	Scene::Scene() {
		
		struct TransformComponent {
			glm::mat4 Transform;

			operator const glm::mat4&() { return Transform; }
		};

		struct SpriteRendererComponent {
			glm::vec4 Color;
		};

		entt::entity entity = _Registry.create();

		auto& transformComponent = _Registry.emplace<TransformComponent>(entity);

		_Registry.get<TransformComponent>(entity);

		auto view = _Registry.view<TransformComponent>();
		for (auto entity : view) {
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
		}

		auto group = _Registry.group<TransformComponent, SpriteRendererComponent>();

		for (auto entity : group) {
			auto&[transfrom, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
		}
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
		glm::mat4* cameraTransform = nullptr;

		auto view = _Registry.view<CameraComponent, TransformComponent>();

		for (auto entity : view) {
			
			auto& [camera, transform] = view.get<CameraComponent, TransformComponent>(entity);
			if (camera.Primary) {
				mainCamera = &camera.Camera;
				cameraTransform = &transform.Transform;
				break;
			}
		}

		if (mainCamera) {
			Uranus::Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = _Registry.group<TransformComponent, SpriteRendererComponent>();

			for (auto entity : group) {
				auto& [transfrom, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Uranus::Renderer2D::DrawQuad(transfrom, sprite.Color);
			}

			Uranus::Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
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
}