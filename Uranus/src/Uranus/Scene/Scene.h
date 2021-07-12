#pragma once

#include "Uranus/Core/Timestep.h"

#include "entt.hpp"

namespace Uranus {

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);
		Entity CreateEntity(const std::string& name = std::string());

		entt::registry& GetRegistry() { return _Registry; }

		friend class Entity;

	private:
		entt::registry _Registry;
	};

}
