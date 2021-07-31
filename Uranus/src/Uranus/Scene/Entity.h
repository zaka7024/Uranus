#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Uranus {

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity entityHandle, Scene* scene);
		Entity(const Entity& other) = default;

	public:

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			auto& component = _Scene->_Registry.emplace<T>(_EntityHandle, std::forward<Args>(args)...);
			_Scene->OnComponenetAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent() const {
			return _Scene->_Registry.get<T>(_EntityHandle);
		}

		template<typename T>
		T& GetOrEmplaceComponent() const {
			return _Scene->_Registry.get_or_emplace<T>(_EntityHandle);
		}

		template<typename T>
		bool HasComponent() {
			return _Scene->_Registry.any_of<T>(_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			_Scene->_Registry.remove<T>(_EntityHandle);
		}

		operator entt::entity() const { return _EntityHandle; }
		operator uint32_t() const { return (uint32_t)_EntityHandle; }
		operator float() const { return (float)_EntityHandle; }
		operator bool() const { return _EntityHandle != entt::null; }

		bool operator== (const Entity & other) {
			return _EntityHandle == other._EntityHandle && _Scene == other._Scene;
		}

		bool operator!= (const Entity& other) {
			//return !operator==(other);
			return !(*this != other);
		}

		void CopyFrom(Entity entity);

	private:
		entt::entity _EntityHandle { entt::null };
		Scene* _Scene = nullptr;
	};

}
