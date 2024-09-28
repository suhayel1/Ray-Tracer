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

int MAX_DEPTH = 4;  // ray starting from eye has depth 1. no more than 3 reflected rays, therefore max = 4.

float backRgb[3];       // background colour
float ambRgb[3];        // ambient intensity

std::vector<Sphere> spheres;            // MAX_SIZE = 15
std::vector<Light> lights;              // MAX_SIZE = 10

std::vector<int> scaleRgb(float rgb[3]) {
    std::vector<int> newRgb;
    float max = -FLT_MAX;

    for (int i = 0; i < 3; i++) if (rgb[i] > max) max = rgb[i];

    if (max > 1.0f) for (int i = 0; i < 3; i++) rgb[i] = rgb[i] / max;

    for (int i = 0; i < 3; i++) {
        newRgb.push_back(std::floor(rgb[i] * 255.0f));

        // if (newRgb[i] > 255) newRgb[i] = 255;
        if (newRgb[i] < 0) newRgb[i] = 0;
    }

    return newRgb;
}

float quadraticSolve(Ray r) {
    float magS = glm::length(r.s);
    float magC = glm::length(r.c);
    float sDotC = glm::dot(r.s, r.c);

    float x = -1.0f * sDotC / (magC*magC);
    float y =  sDotC * sDotC    -    magC * magC * (magS * magS - 1.0f);

    if (y < 0.0f) return -1.0f;         // no solution
    else if (y < 0.0000000001f) return x;    // one solution
    else {
        std::pair<float, float> intersectPair;
        intersectPair.first = x + sqrt(y) / (magC*magC);
        intersectPair.second = x - sqrt(y) / (magC*magC);

        // if (intersectPair.first < 1.0f && intersectPair.second < 1.0f) return -1.0f;
        // else if (intersectPair.first < 1.0f || intersectPair.second < 1.0f) return std::max(intersectPair.first, intersectPair.second);
        // else return std::min(intersectPair.first, intersectPair.second);
        return std::min(intersectPair.first, intersectPair.second);
    }  
}

std::vector<float> findIntersectionTimes(Ray r) {
    std::vector<float> intersectTimes;

    for (Sphere s : spheres) {
        // sphere translation
        glm::vec3 transl(s.pos[0], s.pos[1], s.pos[2]);
        glm::mat4 translMat = glm::translate(glm::mat4(1.0f), transl);

        // sphere scaling
        glm::vec3 scale(s.scl[0], s.scl[1], s.scl[2]);
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);

        // full sphere transformation
        glm::mat4 transfMat = translMat * scaleMat;
        // std::cout << "Transformation Matrix for " << s.name << ":" << std::endl;
        // for (int i = 0; i < 4; ++i) {
        //     for (int j = 0; j < 4; ++j) {
        //         std::cout << transfMat[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }

        Ray invTransfRay(r);

        // check if transformation matrix is invertible
        float determinant = glm::determinant(transfMat);
        if (determinant < 0.0000000001f) {
            std::cout << "Error: Matrix is NOT invertible!" << std::endl;
            return {};
        }

        // Inverse transform ray [(M^-1)(S+ct)] and get S’+c’t
        glm::mat4 invTransfMat = glm::inverse(transfMat);
        glm::vec4 newS(invTransfRay.s, 1.0f);
        newS = invTransfMat * newS; // S'
        //std::cout << "(" << newS.x << ", " << newS.y << ", " << newS.z << ", " << newS.w << ")\n" << std::endl;
        invTransfRay.s = glm::vec3(newS);
        //std::cout << "(" << invTransfRay.s.x << ", " << invTransfRay.s.y << ", " << invTransfRay.s.z << ")\n" << std::endl;
        glm::vec4 newC(invTransfRay.c, 0.0f);
        newC = invTransfMat * newC; // c'
        invTransfRay.c = glm::vec3(newC);

        // find intersections between inv-ray and canonical sphere
        float time = quadraticSolve(invTransfRay);
        intersectTimes.push_back(time);
    }

    return intersectTimes;
}

std::vector<int> rayTrace(Ray r, float tMin, int x, int y){
    r.incDepth();

    if (r.depth > MAX_DEPTH) return {0, 0, 0};

    std::vector<float> intersectTimes = findIntersectionTimes(r);
    if (intersectTimes.size() == 0) return {};

    // find closest intersection 
    Sphere closest;
    float t = FLT_MAX;
    for (int i = 0; i < intersectTimes.size(); i++) {
        if (intersectTimes[i] > 0.0f && intersectTimes[i] < t) {
            closest = spheres[i];
            t = intersectTimes[i];
        }
    }

    glm::vec3 p;
    // if no intersection, return background colour
    if (t == FLT_MAX && r.depth == 1) {
        //std::cout << "No intersection" << std::endl;
        return scaleRgb(backRgb);
    }
    // use 't' in untransformed ray equation to find intersection point 'p'
    else p = r.s + glm::vec3(t) * r.c;

    // std::cout << closest.name << std::endl;
    // std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;

    float pxColour[3] = {0.0f, 0.0f, 0.0f};

    for (int i = 0; i < 3; i++) {
        pxColour[i] = closest.k[0] * ambRgb[i] * closest.rgb[i];
    }

    //if (t < 1.0f) return scaleRgb(pxColour);

    glm::vec3 normal;
    for (Light l : lights) {
        t = FLT_MAX;
        Ray lightRay = Ray();
        lightRay.s = p;
        lightRay.c = glm::normalize(glm::vec3(l.pos[0], l.pos[1], l.pos[2]) - lightRay.s);
        
        std::vector<float> lightRayIntersectTimes = findIntersectionTimes(lightRay);
        if (lightRayIntersectTimes.size() == 0) return {};

        for (int i = 0; i < lightRayIntersectTimes.size() && t == FLT_MAX; i++) {
            if (lightRayIntersectTimes[i] >  0.01f && lightRayIntersectTimes[i] < t) {
                //closest = spheres[i];
                t = lightRayIntersectTimes[i];
            }
            if (lightRayIntersectTimes[i] >  0.01f && lightRayIntersectTimes[i] < t) {
                //closest = spheres[i];
                t = lightRayIntersectTimes[i];
            }
        }

        if (t == FLT_MAX) {
            // sphere translation
            glm::vec3 transl(closest.pos[0], closest.pos[1], closest.pos[2]);
            glm::mat4 translMat = glm::translate(glm::mat4(1.0f), transl);

            // sphere scaling
            glm::vec3 scale(closest.scl[0], closest.scl[1], closest.scl[2]);
            glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);

            // full sphere transformation
            glm::mat4 transfMat = translMat * scaleMat;
            glm::mat4 invTransfMat = glm::inverse(transfMat);
            glm::mat4 invTranspTransfMat = glm::transpose(invTransfMat);

            glm::vec4 localP(p, 1.0f);
            localP = invTransfMat * localP;

            glm::vec3 localNormal = glm::normalize(glm::vec3(2.0f) * (glm::vec3(localP) - glm::vec3(0)));
            glm::vec4 localNormal4(localNormal, 0.0f);
            localNormal4 = invTranspTransfMat * localNormal4;
            normal = glm::normalize(glm::vec3(localNormal4));

            // if (x == 300 && y == 300) {
            //     std::cout << "Intersection: (" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
            //     std::cout << "Normal: (" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
            //     std::cout << "Light: (" << lightRay.c.x << ", " << lightRay.c.y << ", " << lightRay.c.z << ")" << std::endl;
            // }

            float nDotL = glm::dot(normal, lightRay.c);
            //if (nDotL < 0.0f || (normal.z < 0.0f && lightRay.c.z < 0.0f)) {
            if (nDotL < 0.0f) {
                // normal = -normal;
                // nDotL = 0.0f;
                continue;
            }

            for (int i = 0; i < 3; i++) {
                pxColour[i] += closest.k[1] * l.iRgb[i] * nDotL * closest.rgb[i];
            }

            glm::vec3 rSpec = glm::normalize(glm::vec3(2.0f) * nDotL * normal - lightRay.c);
            glm::vec3 view = glm::normalize(glm::vec3(0.0f) - p);

            // if (x == 300 && y == 300) {
            //     std::cout << "rSpec: (" << rSpec.x << ", " << rSpec.y << ", " << rSpec.z << ")" << std::endl;
            //     std::cout << "View: (" << view.x << ", " << view.y << ", " << view.z << ")" << std::endl;
            // }

            for (int i = 0; i < 3; i++) {
                pxColour[i] += closest.k[2] * l.iRgb[i] * std::pow(std::max(glm::dot(rSpec, view), 0.0f), closest.n);
            }
        }
    }

    // glm::vec3 v = glm::normalize(glm::vec3(-2.0f) * glm::dot(normal, r.c) * normal + r.c);
    // Ray ref = Ray();
    // ref.s = p;
    // ref.c = v;
    // ref.depth = 1;

    // std::vector<int> 

    // return scaleRgb(pxColour) + rayTrace(ref, 0.0f, x, y);
    return scaleRgb(pxColour);
}

int main(int argc, char** argv) {
    // check if a file name is inputted
    if (argc != 2) {
        std::cout << "Input Error" << std::endl;
        return 1;
    }
    
    // declare and init input file 
    std::ifstream inputFile(argv[1]);

    // check if file is opened
    if (!inputFile.is_open()) {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int near, left, right, bottom, top;
    int w, h;
    int res[2];             // display resolution

    std::string inputLine;
    std::string outputFile;

    std::vector<std::string> tokens;

    while (std::getline(inputFile, inputLine)) {
        // create an input string stream from the given string
        std::istringstream iss(inputLine);

        // tokenize the string
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens[0] == "NEAR") near = std::stoi(tokens[1]);
        else if (tokens[0] == "LEFT") left = std::stoi(tokens[1]);
        else if (tokens[0] == "RIGHT") right = std::stoi(tokens[1]);
        else if (tokens[0] == "BOTTOM") bottom = std::stoi(tokens[1]);
        else if (tokens[0] == "TOP") top = std::stoi(tokens[1]);
        else if (tokens[0] == "RES") {
            res[0] = std::stoi(tokens[1]);
            res[1] = std::stoi(tokens[2]);
        }
        else if (tokens[0] == "SPHERE") {
            // parse sphere inputs
            float sphPos[3] = {std::stof(tokens[2]), std::stof(tokens[3]), std::stof(tokens[4])};
            float sphScl[3] = {std::stof(tokens[5]), std::stof(tokens[6]), std::stof(tokens[7])};
            float sphRgb[3] = {std::stof(tokens[8]), std::stof(tokens[9]), std::stof(tokens[10])};
            float sphK[4] = {std::stof(tokens[11]), std::stof(tokens[12]), std::stof(tokens[13]), std::stof(tokens[14])};   // [Ka, Kd, Ks, Kr]
            int sphN = std::stoi(tokens[15]);

            // create sphere object and add it to vector of spheres
            Sphere newSphere(tokens[1], sphPos, sphScl, sphRgb, sphK, sphN);
            spheres.push_back(newSphere);
        }
        else if (tokens[0] == "LIGHT") {
            // parse light inputs
            float litPos[3] = {std::stof(tokens[2]), std::stof(tokens[3]), std::stof(tokens[4])};
            float litRgb[3] = {std::stof(tokens[5]), std::stof(tokens[6]), std::stof(tokens[7])};

            // create light obj and add it to vec of light sources
            Light newLight(tokens[1], litPos, litRgb);
            lights.push_back(newLight);
        }
        else if (tokens[0] == "BACK") {
            backRgb[0] = std::stof(tokens[1]);
            backRgb[1] = std::stof(tokens[2]);
            backRgb[2] = std::stof(tokens[3]);
        }
        else if (tokens[0] == "AMBIENT") {
            ambRgb[0] = std::stof(tokens[1]);
            ambRgb[1] = std::stof(tokens[2]);
            ambRgb[2] = std::stof(tokens[3]);
        }
        else if (tokens[0] == "OUTPUT") {
            outputFile = tokens[1];
        }

        // output the tokens
        // for (const auto& t : tokens) {
        //     std::cout << t << std::endl;
        // }
        tokens.clear();     // prepare for next line to read in inputFile
    }

    inputFile.close();

    if (spheres.size() > 15) {
        std::cout << "Error: Sphere count limit passed (" << spheres.size() << " were created)" << std::endl;
        return 1;
    }

    if (lights.size() > 10) {
        std::cout << "Error: Light count limit passed (" << lights.size() << " were created)" << std::endl;
        return 1;
    }

    // output near, left, right, bottom, top
    // std::cout << "NEAR: " << near << std::endl;
    // std::cout << "LEFT: " << left << std::endl;
    // std::cout << "RIGHT: " << right << std::endl;
    // std::cout << "BOTTOM: " << bottom << std::endl;
    // std::cout << "TOP: " << top << "\n" << std::endl;

    // output resolution
    // std::cout << "RES: " << res[0] << " x " << res[1] << "\n" << std::endl;

    // output spheres details
    // for (Sphere s : spheres) {
    //     s.showDetails();
    // }

    // output lights details
    // for (Light l : lights) {
    //     l.showDetails();
    // }

    // output background colour
    //std::cout << "BACK RGB: (" << backRgb[0] << ", " << backRgb[1] << ", " << backRgb[2] << ")\n" << std::endl;

    // output ambient intensity
    //std::cout << "AMBIENT: (" << ambRgb[0] << ", " << ambRgb[1] << ", " << ambRgb[2] << ")\n" << std::endl;

    // output resulting file name
    //std::cout << "OUTPUT: " << outputFile << ")\n" << std::endl;

    Camera cam = Camera();
    //cam.showDetails();

    // pixels colours array for ppm save_imageP3 & save_imageP6
    // p00_r, p00_g, p00_b, p01_r, p01_g, p01_b, ...
    // p10_r, p10_g, p10_b, ...
    // ...
    // float** pixels = new float*[res[0]];
    // for (int i = 0; i < res[0]; i++) {
    //     pixels[i] = new float[res[1] * 3];
    // }
    unsigned char* pixels = new unsigned char[res[0] * res[1] * 3];

    // rays (S+ct) array
    // ray00, ray01, ray02, ...
    // ray10, ray11, ray12, ...
    // ...
    Ray** rays = new Ray*[res[0]];
    for (int j = 0; j < res[0]; j++) {
        rays[j] = new Ray[res[1]];
    }

    w = (right - left) / 2;
    h = (top - bottom) / 2;
    for (int x = 0; x < res[0]; x++) {
        for (int y = 0; y < res[1]; y++) {
            rays[x][y] = Ray(cam, w, h, res[0], res[1], near, x, y);
        }
    }

    std::vector<int> pxColour;

    int z = 0;
    for (int x = 0; x < res[0]; x++) {
        for (int y = 0; y < res[1]; y++) {
            pxColour = rayTrace(rays[x][y], 1.0f, x, y);

            if (pxColour.size() == 0) {
                std::cout << "No colour was returned!" << std::endl;
                return 1;
            }

            // if (x == 140 && y == 300) std::cout << pxColour[0] << " " << pxColour[1] << " " << pxColour[2] << std::endl;
            // if (x == 300 && y == 300) std::cout << pxColour[0] << " " << pxColour[1] << " " << pxColour[2] << std::endl;

            //std::cout << "(" << pxColour[0] << ", " << pxColour[1] << ", " << pxColour[2] << ")" << std::endl;
            pixels[z] = static_cast<unsigned char>(pxColour[0]);
            pixels[z+1] = static_cast<unsigned char>(pxColour[1]);
            pixels[z+2] = static_cast<unsigned char>(pxColour[2]);
            //std::cout << "(" << pixels[0] << ", " << pixels[1] << ", " << pixels[2] << ")" << std::endl;
            z = z + 3;
            //break;
        }
        //break;
    }

    save_imageP6(res[0], res[1], outputFile.c_str(), pixels);

    // DEALLOCATE MEMORY:

    // pixels array
    // for (int i = 0; i < res[0]; ++i) {
    //     delete[] pixels[i];
    // }
    delete[] pixels;

    // rays array
    for (int i = 0; i < res[0]; ++i) {
        delete[] rays[i];
    }
    delete[] rays;

    return 0;
}
