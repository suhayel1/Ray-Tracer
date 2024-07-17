#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "sphere.h"
#include "light.h"
//#include "camera.h"
#include "ray.h"
#include "ppm.cpp"

// max depth of ray constant

// background and ambience colour init.

// vector for sphere and light instances

// function here that scales RGB values from [0.0, 1.0] to [0, 255]

// function here that quadratic solves to find closest intersection point of sphere with ray

// function here that finds the intersection time between specific ray and all sphere objects


std::vector<int> rayTrace(){
    // increase ray depth and return black if exceeds max ray depth

    // find intersection times for the ray

    // find closest intersection 

    // if no intersection, return background colour
    
    // use 't' in untransformed ray equation to find intersection point 'p'

    // declare pixel var

    // initialize pixel colour to color of closest intersected obj * ambience colour * amb. coeff.
    
    // calculate diffuse and specular components and add results to colour

    // return scaled pixel colour
}

int main(int argc, char** argv) {
    // check if a file name is inputted
    
    // declare and init input file 

    // check if file is opened

    // declare variables for input file parsing
    
    // declare input and output file

    // parse input file and assign values to declared variables

    //close input file

    // check if sphere count exceeds max amount

    // check if light source count exceeds max amount
   
    // create camera obj

    // create pixels array

    // create rays array

    // create new Ray instance for each element in rays array

    // declare pixel colour var

    // call raytrace function to calculate each pixel value

    // call function to save pixels in P6 PPM file

    // deallocate memory for pixels and rays array
    
    return 0;
}
