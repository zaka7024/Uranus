#include "urpch.h"

#include "Entity.h"
#include "Components.h"

namespace Uranus {

	Entity::Entity(entt::entity entityHandle, Scene* scene)
		: _EntityHandle(entityHandle), _Scene(scene) 
	{

	}

	template<typename T>
	static void CopyComponent(Entity entity, Entity other) {
		if (other.HasComponent<T>()) {
			auto& component = entity.GetOrEmplaceComponent<T>();
			component = other.GetComponent<T>();
		}
	}

	void Entity::CopyFrom(Entity entity)
	{
		CopyComponent<TagComponent>(*this, entity);
		CopyComponent<TransformComponent>(*this, entity);
		CopyComponent<CameraComponent>(*this, entity);
		CopyComponent<SpriteRendererComponent>(*this, entity);
	}
}
