#ifndef CAMERA_H
#define CAMERA

#include <iostream>
#include <string>

#include "glm/glm.hpp"

class Camera {
public:
    glm::vec3 eye, u, v, n;

    Camera();

    void showVec(glm::vec3 v);
    void showDetails();
};

#endif
