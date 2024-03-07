#include <glm/glm.hpp>

#pragma once
struct Vertex
{
public:
	glm::vec3 positions;
	glm::vec3 normals;
	glm::vec2 texCoords;
	glm::vec4 color;
	glm::vec4 boneWeights;
	glm::vec4 boneIds;

	float enabled = 0;

	/*glm::vec3 tangents;
	glm::vec3 bitTangents;*/
};

