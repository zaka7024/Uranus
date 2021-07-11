#include "urpch.h"
#include "Scene.h"
#include "Components.h"

#include "Uranus/Renderer/Renderer2D.h"

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

		entt::entity entity = _registry.create();

		auto& transformComponent = _registry.emplace<TransformComponent>(entity);

		_registry.get<TransformComponent>(entity);

		auto view = _registry.view<TransformComponent>();
		for (auto entity : view) {
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
		}

		auto group = _registry.group<TransformComponent, SpriteRendererComponent>();

		for (auto entity : group) {
			auto&[transfrom, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
		}
	}

	Scene::~Scene() {

	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = _registry.group<TransformComponent, SpriteRendererComponent>();

		for (auto entity : group) {
			auto& [transfrom, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Uranus::Renderer2D::DrawQuad(transfrom, sprite.Color);
		}
	}

	entt::entity Scene::CreateEntity()
	{
		return entt::entity();
	}
}