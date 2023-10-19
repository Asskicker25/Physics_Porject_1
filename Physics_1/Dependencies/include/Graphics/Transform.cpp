#include "Transform.h"

Transform::Transform() : position{ glm::vec3(0) }, rotation{ glm::vec3(0) }, scale{ glm::vec3(1.0f) }
{
}

Transform::Transform(const Transform& transform)
{
	position = transform.position;
	rotation = transform.rotation;
	scale = transform.scale;
}

void Transform::SetPosition(glm::vec3 _position)
{
	position = _position;
}

void Transform::SetRotation(glm::vec3 _rotation)
{
	rotation = _rotation;
}

void Transform::SetScale(glm::vec3 _scale)
{
	scale = _scale;
}

glm::mat4 Transform::GetTransformMatrix()
{
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, position);

	trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0, 0, 1));

	trans = glm::scale(trans, scale);

	return trans;
}

glm::mat4 Transform::GetInverseMatrix()
{
	return glm::inverse(glm::transpose(GetTransformMatrix()));
}

glm::vec3 Transform::GetForward()
{
	return glm::normalize(-glm::vec3(GetTransformMatrix()[2]));
}

glm::vec3 Transform::GetUp()
{
	return glm::normalize(glm::vec3(GetTransformMatrix()[1]));
}

glm::vec3 Transform::GetRight()
{
	return glm::normalize(glm::vec3(GetTransformMatrix()[0]));
}
