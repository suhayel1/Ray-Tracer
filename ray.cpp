#include "ray.h"

Ray::Ray() {}

Ray::Ray(const Ray& ray) {
    this->s = ray.s;
    this->c = ray.c;
    this->depth = ray.depth;
}

Ray::Ray(Camera cam, int w, int h, int numCols, int numRows, int near, int r, int c)
   : Ray(cam.eye, cam.u, cam.v, cam.n, w, h, numCols, numRows, near, r, c) {
}

Ray::Ray(glm::vec3 s, glm::vec3 u, glm::vec3 v, glm::vec3 n, int w, int h, int numCols, int numRows, int near, int row, int col) {
    this->depth = 0;
    this->s = s;
    this->c =  glm::vec3(-1.0f * near) * n     +     glm::vec3(w * (2.0f*col / numCols - 1.0f)) * u     +     glm::vec3(h * (2.0f*row / numRows - 1.0f)) * v;
    if (this->c.y != 0) this->c.y = this->c.y * -1.0f;
    this->c = glm::normalize(this->c);
}

void Ray::incDepth() {
    this->depth++;
}

void Ray::showDetails() {
    std::cout << "S" << std::endl;
    std::cout << "=" << std::endl;
    std::cout << "(" << s.x << ", " << s.y << ", " << s.z << ")" << std::endl;
    std::cout << "C" << std::endl;
    std::cout << "=" << std::endl;
    std::cout << "(" << c.x << ", " << c.y << ", " << c.z << ")" << std::endl;
    std::cout << "DEPTH" << std::endl;
    std::cout << "=====" << std::endl;
    std::cout << this->depth << "\n" << std::endl;
}
