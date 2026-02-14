#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "texture.h"
#include "polygon.h"

Texture::Texture(int newId) : Polygon(newId) {
    this->isTextured = true;
}

Texture::~Texture() {

}

void Texture::GetPixel(double cpx, double cpy, double& r, double& g, double& b) {
    //cpx definitely greater than minX, cpy definitely smaller than maxY
    double xPercent = (cpx - minX) / this->realWidth;
    double yPercent = (maxY - cpy) / this->realHeight;

    int x = (int)std::floor(xPercent * (this->width - 1));
    int y = (int)std::floor(yPercent * (this->height - 1));

    // handles bmp's stored bottom to top
    int offset = ((height - 1 - y) * stride) + (x * 3);
    b = pixels[offset];
    g = pixels[offset + 1];
    r = pixels[offset + 2];
}

//texture name must be set first!
void Texture::LoadBMP() {
    if(this->textureName == "") {
        std::cout << "No texture name!" << std::endl;
        exit(1);
    }
    std::cout << "Loading texture: " << this->textureName << std::endl;

    std::ifstream f("Resources\\Textures\\" + textureName + ".bmp", std::ios::binary);

    if (!f) {
        std::cout << "Texture not found!" << std::endl;
        exit(1);
    }

    unsigned char header[54];
    f.read((char*)header, 54);

    this->width  = *(int*)&header[18];
    this->height = *(int*)&header[22];

    //stride is row size with padding
    this->stride = ((this->width * 24 + 31) / 32) * 4;

    int dataSize = this->stride * this->height;
    std::vector<unsigned char> tempPixels;
    tempPixels.resize(dataSize);
    this->pixels.resize(dataSize);
    
    // Jump to the pixel data offset
    int pixelOffset = *(int*)&header[10];
    f.seekg(pixelOffset, std::ios::beg);
    
    f.read((char*)(tempPixels.data()), dataSize);
    f.close();

    for(int i = 0; i < (int)(tempPixels.size()); i++) {
        this->pixels[i] = (double)(tempPixels[i]) / ((double)MAX_COLOR);
    }
    std::cout << "Finished."  << std::endl;
}

void Texture::Print() {
    std::cout << "Texture (" << textureName << "): " << std::endl;
    for(int i = 0; i < numPoints; i++) {
        std::cout << i << ": "
                  << points[i*3] << " "
                  << points[i*3 + 1] << " "
                  << points[i*3 + 2] << " "
                  << std::endl;
    }
    std::cout << "Normal: " << nx << " " << ny << " " << nz << std::endl;
    this->material->Print();
}