#include "urpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Uranus {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), _ViewMatrix(1.0f)
	{
		_ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, _Position);
		trans = glm::rotate(trans, glm::radians(_Rotation), glm::vec3(0, 1, 0));

		_ViewMatrix = glm::inverse(trans);
		_ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	}
}
