#pragma once

#include <glm/glm.hpp>

struct AABB
{
	glm::vec3 min; 
	glm::vec3 max;
};

static AABB CalculateAABB(const std::vector<Vertex>& vertices)
{
    if (vertices.size() == 0)
    {
	    return AABB{ glm::vec3(0.0f), glm::vec3(0.0f) };
    }

    glm::vec3 min = vertices[0].positions;
    glm::vec3 max = vertices[0].positions;

    for (const Vertex& vertex : vertices) 
    {
        min.x = std::min(min.x, vertex.positions.x);
        min.y = std::min(min.y, vertex.positions.y);
        min.z = std::min(min.z, vertex.positions.z);

        max.x = std::max(max.x, vertex.positions.x);
        max.y = std::max(max.y, vertex.positions.y);
        max.z = std::max(max.z, vertex.positions.z);
    }

    return AABB{ min, max };

}

static AABB CombineAABBs(const std::vector<AABB>& aabbs) 
{
    if (aabbs.empty()) 
    {
        return AABB{ glm::vec3(0.0f), glm::vec3(0.0f) };
    }

    glm::vec3 min = aabbs[0].min;
    glm::vec3 max = aabbs[0].max;

    for (const AABB& aabb : aabbs) {
        min.x = std::min(min.x, aabb.min.x);
        min.y = std::min(min.y, aabb.min.y);
        min.z = std::min(min.z, aabb.min.z);

        max.x = std::max(max.x, aabb.max.x);
        max.y = std::max(max.y, aabb.max.y);
        max.z = std::max(max.z, aabb.max.z);
    }

    return AABB{ min, max };
}

static bool CollisionAABBvsAABB(AABB a, AABB b)
{
    // Exit with no intersection if separated along an axis
    if (a.max[0] < b.min[0] || a.min[0] > b.max[0]) return false;
    if (a.max[1] < b.min[1] || a.min[1] > b.max[1]) return false;
    if (a.max[2] < b.min[2] || a.min[2] > b.max[2]) return false;
    // Overlapping on all axes means AABBs are intersecting
    return true;
}