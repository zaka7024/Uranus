#pragma once

#include <glm/glm.hpp>

#include "Uranus/Scene/SceneCamera.h"
#include "Uranus/Scene/ScriptableEntity.h"

namespace Uranus {

	struct TagComponent {
		std::string Tag = std::string();

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};


	struct TransformComponent {
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transfrom)
			: Transform(transfrom) {}

		operator const glm::mat4& () { return Transform; }
	};

	struct SpriteRendererComponent {
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent {
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent {
		ScriptableEntity* Instance = nullptr;

		NativeScriptComponent() = default;

		//ScriptableEntity*(*InstantiateScript)();
		std::function<ScriptableEntity*()> InstantiateScript;
		void(*DestroyScript)(NativeScriptComponent*);
		
		template<typename T>
		void Bind() {
			InstantiateScript = []() {
				return new T();
			};

			DestroyScript = [](NativeScriptComponent* nsc) {
				delete nsc->Instance; nsc->Instance = nullptr;
			};
		}
	};
}
