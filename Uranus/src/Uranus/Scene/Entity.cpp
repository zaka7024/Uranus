#include "urpch.h"

#include "Entity.h"

namespace Uranus {

	Entity::Entity(entt::entity entityHandle, Scene* scene)
		: _EntityHandle(entityHandle), _Scene(scene) 
	{

	}

}