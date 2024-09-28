#include "camera.h"

Camera::Camera() {  // right-handed
    this->eye = glm::vec3(0.0f);
    this->u = glm::vec3(1.0f, 0.0f, 0.0f);
    this->v = glm::vec3(0.0f, 1.0f, 0.0f);
    this->n = glm::vec3(0.0f, 0.0f, 1.0f);
}

void Camera::showVec(glm::vec3 v) {
    std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
}

void Camera::showDetails() {
    std::cout << "EYE: ";
    Camera::showVec(this->eye);
    std::cout << "U: ";
    Camera::showVec(this->u);
    std::cout << "V: ";
    Camera::showVec(this->v);
    std::cout << "N: ";
    Camera::showVec(this->n);
    std::cout << std::endl;
}
