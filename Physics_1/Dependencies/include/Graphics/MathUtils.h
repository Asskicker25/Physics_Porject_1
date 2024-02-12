#pragma once

#include <glm/glm.hpp>

class MathUtils
{
public:

	static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& pos, glm::vec3& rot, glm::vec3& scale);
};

