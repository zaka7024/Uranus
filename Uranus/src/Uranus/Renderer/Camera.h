#pragma once

#include <glm/glm.hpp>

namespace Uranus {

	class Camera {
	public:
		Camera(const glm::mat4 projection)
			:_Projection(projection) {}

		const glm::mat4& GetProjection() { return _Projection; }
	private:
		glm::mat4 _Projection;
	};

}