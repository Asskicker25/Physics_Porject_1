#pragma once

#include <glm/glm.hpp>

enum PhysicsShape
{
    SPHERE,
    PLANE,
    TRIANGLE,
    AABB,
    CAPSULE,
    MESH_OF_TRIANGLES_INDIRECT,
    MESH_OF_TRIANGLES_LOCAL_VERTICES,
};

enum PhysicsMode
{
    STATIC,
    DYNAMIC,
    KINEMATIC,
};

struct iShape
{
    virtual ~iShape() {}
    
};

struct Aabb : public iShape
{
    Aabb() {}
    Aabb(glm::vec3 min,glm::vec3 max)
    {
        this->min = min;
        this->max = max;
    }
	glm::vec3 min; 
	glm::vec3 max;
};

struct Sphere : public iShape
{
    Sphere() {}
    Sphere(glm::vec3 position, float radius)
    {
        this->position = position;
        this->radius = radius;
    }

    glm::vec3 position;
    float radius;
};

struct Triangle : public iShape
{
    Triangle() {}
    Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
    {
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
    }

    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 v3;
};

static Aabb CalculateAABB(const std::vector<Vertex>& vertices)
{
    if (vertices.size() == 0)
    {
	    return Aabb{ glm::vec3(0.0f), glm::vec3(0.0f) };
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

    return Aabb{ min, max };
}

//static Sphere CalculateSphere(const std::vector<Vertex>& vertices)
//{
//    if (vertices.empty()) {
//        return Sphere{ glm::vec3(0.0f), -1.0f };
//    }
//
//    // Initialize the min and max points.
//    glm::vec3 minPoint = vertices[0].positions;
//    glm::vec3 maxPoint = vertices[0].positions;
//
//    for (const Vertex& vertex : vertices) {
//        minPoint = glm::min(minPoint, vertex.positions);
//        maxPoint = glm::max(maxPoint, vertex.positions);
//    }
//
//    glm::vec3 center = 0.5f * (minPoint + maxPoint);
//
//    float radius = glm::length(maxPoint - center);
//
//    return Sphere{ center, radius };
//}

static bool CollisionAABBvsAABB(const Aabb& a, const Aabb& b)
{
    // Exit with no intersection if separated along an axis
    if (a.max[0] < b.min[0] || a.min[0] > b.max[0]) return false;
    if (a.max[1] < b.min[1] || a.min[1] > b.max[1]) return false;
    if (a.max[2] < b.min[2] || a.min[2] > b.max[2]) return false;

    std::cout << "AABB VS AABBBBBBB" << std::endl;
    // Overlapping on all axes means AABBs are intersecting
    return true;
}

// Computes the square distance between a point p and an AABB b
static float SqDistPointAABB(glm::vec3 p, Aabb b)
{
    float sqDist = 0.0f;
    for (int i = 0; i < 3; i++) 
    {
        // For each axis count any excess distance outside box extents
        float v = p[i];
        if (v < b.min[i])
            sqDist += (b.min[i] - v) * (b.min[i] - v);
        if (v > b.max[i]) 
            sqDist += (v - b.max[i]) * (v - b.max[i]);
    }
    return sqDist;
}

static bool CollisionSpherevsAABB(Sphere* sphere, const Aabb& aabb)
{
    // Compute squared distance between sphere center and AABB
    float sqDist = SqDistPointAABB(sphere->position, aabb);

   /* std::cout << "Square Position : " << sphere->position.y << std::endl;
    std::cout << "Square Distance : " << sqDist << std::endl;
    std::cout << "Square Radius : " << sphere->radius * sphere->radius << std::endl;*/

    // Sphere and AABB intersect if the (squared) distance
    // between them is less than the (squared) sphere radius
    return sqDist <= sphere->radius * sphere->radius;
}

//static bool CollisionSpherevsTriangle(Sphere* sphere, )
//{
//
//}