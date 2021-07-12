#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Uranus {

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity entityHandle, Scene* scene);
		Entity(const Entity&) = default;

	public:

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			return _Scene->_Registry.emplace<T>(_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		T& GetComponent() {
			return _Scene->_Registry.get<T>(_EntityHandle);
		}

		template<typename T>
		bool HasComponent() {
			return _Scene->_Registry.any_of<T>(_EntityHandle);
		}

		operator bool() const { return _EntityHandle != entt::null; }

	private:
		entt::entity _EntityHandle { entt::null };
		Scene* _Scene = nullptr;
	};

}
