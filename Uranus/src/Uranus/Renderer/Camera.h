#pragma once

#include <glm/glm.hpp>

namespace Uranus {

	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4 projection)
			:_Projection(projection) {}

		const glm::mat4& GetProjection() { return _Projection; }
	protected:
		glm::mat4 _Projection;
		float _AspectRatio = 0.0f;
	};

}