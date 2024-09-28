#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include <string>

class Sphere {
public:
    std::string name;
    float pos[3];
    float scl[3];
    float rgb[3];
    float k[4]; // [Ka, Kd, Ks, Kr]
    int n;

    Sphere();
    Sphere(std::string name, float pos[3], float scl[3], float rgb[3], float k[4], int n);

    void showDetails();
};

#endif
