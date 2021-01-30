#pragma once

#include <glm/glm.hpp>

namespace Uranus {

	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		inline void SetPoisition(const glm::vec3& position) {
			_Position = position; RecalculateViewMatrix();
		}

		inline glm::vec3& GetPoisition() { return _Position; }

		inline void SetRotation(float rotation) { 
			_Rotation = rotation; RecalculateViewMatrix();
		}

		inline float GetRotation() { return _Rotation; }

		const glm::mat4& GetViewMatrix() { return _ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() { return _ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() { return _ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 _ViewMatrix;
		glm::mat4 _ProjectionMatrix;
		glm::mat4 _ViewProjectionMatrix;

		glm::vec3 _Position = {0.0f, 0.0f, 0.0f};
		float _Rotation = 0.0f;
	};
}