#ifndef MATH_UTILITIES_H_
#define MATH_UTILITIES_H_

class MathUtilities {
public:
    static unsigned char ColorAmp(double percent);
    static void Normalize(double& dx, double& dy, double& dz);
    static double DotProduct(double x1, double y1, double z1, double x2, double y2, double z2);
private:
    MathUtilities() = default;
};

#endif