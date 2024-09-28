#include "light.h"

Light::Light(std::string name, float pos[3], float iRgb[3]) {
    this->name = name;

    for (int i = 0; i < 3; i++) {
        this->pos[i] = pos[i];
        this->iRgb[i] = iRgb[i];
    }
}

void Light::showDetails() {
    std::cout << "LIGHT" << std::endl;
    std::cout << "=====" << std::endl;
    std::cout << "Name: " << this->name << std::endl;
    std::cout << "Position: (" << this->pos[0] << ", " << this->pos[1] << ", " << this->pos[2] << ")" << std::endl;
    std::cout << "Intensity (RGB): (" << this->iRgb[0] << ", " << this->iRgb[1] << ", " << this->iRgb[2] << ")\n" << std::endl;
}
