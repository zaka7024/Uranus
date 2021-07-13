#pragma once

#include "Entity.h"
#include "Scene.h"

namespace Uranus {

	class ScriptableEntity {
	public:
		template<typename T>
		T& GetComponent() {
			return _Entity.GetComponent<T>();
		}

		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(Timestep ts) {};

	private:
		Entity _Entity;
		friend class Scene;
	};
}