#include "Primitives.h"

#include <algorithm>

void Mesh2D::addVertex(glm::vec2 point) {
    verticies.push_back(point);
}

void Mesh2D::setOrigin(glm::vec2 point) {
    origin = point;
}

void Mesh3D::addVertex(glm::vec3 point) {
    verticies.push_back(point);
}

void Mesh3D::setOrigin(glm::vec3 point) {
    origin = point;
}

void AABB::update() {
    auto it = _targetMesh->verticies.begin();
    glm::vec3 maxVals, minVals;
    maxVals.x = it->x; // Set the min to the max so that the bounding
    maxVals.y = it->y; // box can be made as big as the farthest point
    maxVals.z = it->z; // on the mesh on a given axis

    minVals.x = maxVals.x;
    minVals.y = maxVals.y;
    minVals.z = maxVals.z;

    for(it = it + 1; it != _targetMesh->verticies.end(); it++) {
        maxVals.x = std::max(maxVals.x, it->x);
        maxVals.y = std::max(maxVals.y, it->y);
        maxVals.z = std::max(maxVals.z, it->z);

        minVals.x = std::min(minVals.x, it->x);
        minVals.y = std::min(minVals.y, it->y);
        minVals.z = std::min(minVals.z, it->z);
    }

    _min = minVals;
    _max = maxVals;
}

