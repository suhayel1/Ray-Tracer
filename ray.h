#ifndef RAY_H
#define RAY

#include <iostream>
#include <string>

#include "glm/glm.hpp"
#include "camera.h"

class Ray {
public:
    // variables

    Ray();
    // overloaded constructors here

    void incDepth();
    void showDetails();
};

#endif
