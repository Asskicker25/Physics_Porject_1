#pragma once

#include <glm/glm.hpp>

enum PhysicsShape
{
    SPHERE,
    PLANE,
    TRIANGLE,
    AABB,
    CAPSULE,
    MESH_OF_TRIANGLES,
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

static bool CollisionSphereVSSphere(Sphere* sphere1, Sphere* sphere2)
{
    glm::vec3 d = sphere1->position - sphere2->position;
    float dist2 = glm::dot(d, d);

    float radiusSum = sphere1->radius + sphere2->radius;
   
    return dist2 <= radiusSum * radiusSum;
}

static glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    glm::vec3 bc = c - b;

    // Compute parametric position s for projection P' of P on AB,
    // P' = A + s*AB, s = snom/(snom+sdenom)
    float snom = glm::dot(p - a, ab), sdenom = glm::dot(p - b, a - b);

    // Compute parametric position t for projection P' of P on AC,
    // P' = A + t*AC, s = tnom/(tnom+tdenom)
    float tnom = glm::dot(p - a, ac), tdenom = glm::dot(p - c, a - c);

    if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out

    // Compute parametric position u for projection P' of P on BC,
    // P' = B + u*BC, u = unom/(unom+udenom)
    float unom = glm::dot(p - b, bc), udenom = glm::dot(p - c, b - c);

    if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
    if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out


    // P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
    glm::vec3 n = glm::cross(b - a, c - a);
    float vc = glm::dot(n, glm::cross(a - p, b - p));
    // If P outside AB and within feature region of AB,
    // return projection of P onto AB
    if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
        return a + snom / (snom + sdenom) * ab;

    // P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
    float va = glm::dot(n, glm::cross(b - p, c - p));
    // If P outside BC and within feature region of BC,
    // return projection of P onto BC
    if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
        return b + unom / (unom + udenom) * bc;

    // P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
    float vb = glm::dot(n, glm::cross(c - p, a - p));
    // If P outside CA and within feature region of CA,
    // return projection of P onto CA
    if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
        return a + tnom / (tnom + tdenom) * ac;

    // P must project inside face region. Compute Q using barycentric coordinates
    float u = va / (va + vb + vc);
    float v = vb / (va + vb + vc);
    float w = 1.0f - u - v; // = vc / (va + vb + vc)
    return u * a + v * b + w * c;
}

static bool CollisionSphereVsTriangle(Sphere* sphere, const Triangle& triangle, glm::vec3& collisionPoint)
{
    collisionPoint = ClosestPtPointTriangle(sphere->position, triangle.v1, triangle.v2, triangle.v3);

    glm::vec3 v = collisionPoint - sphere->position;
    float squaredDistance = glm::dot(v, v);

    return squaredDistance <= (sphere->radius * sphere->radius);
}

static bool CollisionSphereVsMeshOfTriangles(Sphere* sphere,
    const glm::mat4& transformMatrix,
    const std::vector <std::vector <Triangle>>& triangles,
    const std::vector<std::vector<Sphere*>>& triangleSpheres,
    std::vector<glm::vec3>& collisionPoints)
{

    float maxScale = glm::max(glm::max(transformMatrix[0][0], transformMatrix[1][1]), transformMatrix[2][2]);

    collisionPoints.clear();
    Sphere* sphereTriangle = new Sphere();

    for (size_t i = 0; i < triangles.size(); i++) 
    {
        const std::vector<Triangle>& triangleList = triangles[i];
        const std::vector<Sphere*>& sphereList = triangleSpheres[i];

        for (size_t j = 0; j < triangleList.size(); j++) 
        {
            const Triangle& triangle = triangleList[j];

            // Transform the sphere's position using the transformMatrix
            glm::vec4 transformedCenter = transformMatrix * glm::vec4(sphereList[j]->position, 1.0f);
            sphereTriangle->position = glm::vec3(transformedCenter);

            // Transform the sphere's radius based on scaling
            sphereTriangle->radius = sphereList[j]->radius * maxScale;

            // Now you can check for collision between the transformed sphere and sphereTriangle
            if (CollisionSphereVSSphere(sphere, sphereTriangle)) 
            {
                glm::vec3 point = glm::vec3(0.0f);

                if (CollisionSphereVsTriangle(sphere, triangle, point))
                {
                    collisionPoints.push_back(point);
                }
            }
        }
    }

    delete sphereTriangle;


    if (collisionPoints.size() > 0)
        return true;
    //std::cout << "Size : " << collisionPoints.size()<<std::endl;

    return false;
}

