#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Mesh2D {
    void addVertex(glm::vec2 point);

    void setOrigin(glm::vec2 point);

    glm::vec2 origin;
    std::vector<glm::vec2> verticies;
};

struct Mesh3D {
    void addVertex(glm::vec3 point);

    void setOrigin(glm::vec3 point);

    glm::vec3 origin;
    std::vector<glm::vec3> verticies;
};

struct AABB {
    explicit AABB(Mesh3D* mesh) : _targetMesh(mesh) {  update(); }

    void update();

private:
    glm::vec3 _min, _max;

    Mesh3D* _targetMesh;
    Mesh3D _aabbMesh;
};

class Body {
    
private:

};