#pragma once

#include "Uranus/Core/Timestep.h"

#include "entt.hpp"

namespace Uranus {

	class Scene {
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);
		entt::entity CreateEntity();

		entt::registry& GetRegistry() { return _registry; }

	private:
		entt::registry _registry;
	};

}