#include "sphere.h"

Sphere::Sphere(){}

Sphere::Sphere(std::string name, float pos[3], float scl[3], float rgb[3], float k[4], int n) {
    this->name = name;

    for (int i = 0; i < 3; i++) {
        this->pos[i] = pos[i];
        this->scl[i] = scl[i];
        this->rgb[i] = rgb[i];
    }

    for (int i = 0; i < 4; i++) {
        this->k[i] = k[i];
    }
    
    this->n = n;
}

void Sphere::showDetails() {
    std::cout << "SPHERE" << std::endl;
    std::cout << "======" << std::endl;
    std::cout << "Name: " << this->name << std::endl;
    std::cout << "Position: (" << this->pos[0] << ", " << this->pos[1] << ", " << this->pos[2] << ")" << std::endl;
    std::cout << "Scale: (" << this->scl[0] << ", " << this->scl[1] << ", " << this->scl[2] << ")" << std::endl;
    std::cout << "RGB: (" << this->rgb[0] << ", " << this->rgb[1] << ", " << this->rgb[2] << ")" << std::endl;
    std::cout << "K_a, K_d, K_s, K_r: " << this->k[0] << ", " << this->k[1] << ", " << this->k[2] << ", " << this->k[3] << std::endl;
    std::cout << "Specular Exponent: " << this->n << "\n" << std::endl;
}
