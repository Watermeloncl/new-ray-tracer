#ifndef OBJECTS_SCENE_OBJECTS_TEXTURE_H_
#define OBJECTS_SCENE_OBJECTS_TEXTURE_H_

#include <string>
#include <vector>

#include "polygon.h"

struct Texture : public Polygon {
public:
    Texture(int newId);
    ~Texture();

    std::string textureName;

    double realWidth;
    double realHeight;
    double minX;
    double maxY;

    int width;
    int height;
    int stride;
    std::vector<double> pixels;

    void GetPixel(double cpx, double cpy, double& r, double& g, double& b);
    void LoadBMP();

    void Print();  
};

#endif