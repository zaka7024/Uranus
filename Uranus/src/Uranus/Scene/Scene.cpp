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
		auto group = _Registry.group<TransformComponent, SpriteRendererComponent>();

		for (auto entity : group) {
			auto& [transfrom, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Uranus::Renderer2D::DrawQuad(transfrom, sprite.Color);
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