#pragma once

#include "Uranus/Core/Core.h"
#include "Scene.h"

namespace Uranus {

	class SceneSerializer {
	public:
		SceneSerializer(const Ref<Scene>& scene);

	public:
		void Serialize(const std::string& filepath);
		bool Deserialize(const std::string& filepath);

	private:
		Ref<Scene> _Scene;
	};
}