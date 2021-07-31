#pragma once

#include "Uranus/Core/Timestep.h"
#include "Uranus/Renderer/EditorCamera.h"

#include "entt.hpp"

namespace Uranus {

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity CreateEntity(const std::string& name = std::string());
		void DeleteEntity(Entity entity);

		template<typename T>
		void OnComponenetAdded(Entity entity, T& component);

		entt::registry& GetRegistry() { return _Registry; }

		Entity GetPrimaryCameraEntity();

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

	private:
		entt::registry _Registry;
		float _ViewPortWidth, _ViewPortHeight;
	};
}
