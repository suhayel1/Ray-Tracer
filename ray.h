#ifndef RAY_H
#define RAY

#include <iostream>
#include <string>

#include "glm/glm.hpp"
#include "camera.h"

class Ray {
public:
    glm::vec3 s, c;
    int depth;

    Ray();
    Ray(const Ray& ray);
    Ray(Camera cam, int w, int h, int numCols, int numRows, int near, int r, int c);
    Ray(glm::vec3 s, glm::vec3 u, glm::vec3 v, glm::vec3 n, int w, int h, int numCols, int numRows, int near, int r, int c);

    void incDepth();
    void showDetails();
};

#endif
