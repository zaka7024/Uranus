#pragma once
#include "OrthographicCamera.h"

namespace Uranus {

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float roation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float roation, const glm::vec4& color);
	};

}