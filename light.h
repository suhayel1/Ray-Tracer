#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include <string>

class Light {
public:
    std::string name;
    float pos[3];
    float iRgb[3];

    Light(std::string name, float pos[3], float iRgb[3]);

    void showDetails();
};

#endif
