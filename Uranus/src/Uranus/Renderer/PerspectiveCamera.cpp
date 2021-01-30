#include "urpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Uranus {

	PerspectiveCamera::PerspectiveCamera()
		:_ProjectionMatrix(glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f)), _ViewMatrix(1.0f)
	{
		_ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, _Position);
		trans = glm::rotate(trans, glm::radians(_Rotation), glm::vec3(0, 1, 0));

		_ViewMatrix = glm::inverse(trans);
		_ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	}
}